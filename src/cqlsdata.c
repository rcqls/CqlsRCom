#include <stdio.h>
#include "cqlsR.h"
/*
il y a 2 manières d'utiliser le datacenter:
1) on déclare toutes les variables puis on génère les nouvelles données pour autant d'objets que nécessaire!!!
2) on déclare au fur et à mesure et les data inclus dans le datacenter sont allouées au fur et à mesure!!!
*/

DATA_CENTER cqlsdataCenter_new() {
  DATA_CENTER self;

  self=(DATA_CENTER)Calloc(1,ST_DATA_CENTER);
  self->n=0;
  self->type=NULL;
  self->data=NULL;
  cqlsdataCenter_curObj(self,self);
  return self;
}

void cqlsdataCenter_free(DATA_CENTER self) {
  if(self!=(DATA_CENTER)NULL) {
    //free data of the self!!
    self->curObj=(DATA)self;
    cqlsdataCenter_freeData(self);
    //free type
    Free(self->type);
    Free(self);
  }
}

void cqlsdataCenter_show(DATA_CENTER self) {
  int id;
  ST_DATA_STRUCT type;

  Rprintf("dc->n=%d\n",self->n);
  for(id=0;id<self->n;id++) {
    type=self->type[id];
    Rprintf("id=%d->%s=(type=%d,size=%d)\n",id,type.key,type.type,type.size);
  }
}

int cqlsdataCenter_curObj(DATA_CENTER self, DATA obj) {
  self->curObj=obj;
  return ((obj->data) !=NULL);
}

//alloc memory for the last data inside the datacenter!!!
void cqlsdataCenter_allocLastData(DATA_CENTER self) {
  int id;
  
  self->data=(void**)Realloc(self->data,self->n,void*);
  id=self->n - 1;//the last data inside the datacenter
  switch((self->type[id]).type) {
    case DATA_DBL:
      self->data[id]=(void*)Calloc((self->type[id]).size,DOUBLE);
      break;
    case DATA_INT:
      self->data[id]=(void*)Calloc((self->type[id]).size,int);
  }
}


int cqlsdataCenter_declare(DATA_CENTER self,int size,DATA_TYPE type,char *key) {
  int id;
  id=self->n;
  
  (self->n)++;
  self->type=(DATA_STRUCT)Realloc(self->type,self->n,ST_DATA_STRUCT);
  (self->type[id]).id=id;
  (self->type[id]).key=key;
  (self->type[id]).size=size;
  (self->type[id]).type=type;
  //alloc dynamically the data inside the datacenter
  cqlsdataCenter_allocLastData(self);
  return id;
}


//for the curObj not the data in the datacenter!!!
void cqlsdataCenter_newData(DATA_CENTER self) {
  int id;
  void **data;
  
  data=(void**)Calloc(self->n,void*);
  for(id=0;id<self->n;id++) {
    switch((self->type[id]).type) {
    case DATA_DBL: 
      data[id]=(void*)Calloc((self->type[id]).size,DOUBLE);//printf("data[%d]\n",id);
	    break;
    case DATA_INT:
      data[id]=(void*)Calloc((self->type[id]).size,int);//printf("data[%d]\n",id);
    }
  }
  self->curObj->data=data;
}


void cqlsdataCenter_freeData(DATA_CENTER self) {
  int id;
  if(self->curObj->data == NULL) return;
  for(id=0;id<self->n;id++) {
    Free(self->curObj->data[id]);
  }
  Free(self->curObj->data);
  self->curObj->data=NULL;
}

void cqlsdataCenter_newDataForObj(DATA_CENTER self,DATA obj) {
  cqlsdataCenter_curObj(self,obj);
  cqlsdataCenter_newData(self);
}

void cqlsdataCenter_freeDataForObj(DATA_CENTER self,DATA obj) {
  cqlsdataCenter_curObj(self,obj);
  cqlsdataCenter_freeData(self);
}

DOUBLE* cqlsdataCenter_DBL(DATA_CENTER self,int id) {
  return cqlsdata_DBL(self->curObj,id);
}

int* cqlsdataCenter_INT(DATA_CENTER self, int id) {
  return cqlsdata_INT(self->curObj,id);
}

int cqlsdataCenter_size(DATA_CENTER self, int id) {
  return (self->type[id]).size;
}

int cqlsdataCenter_idData(DATA_CENTER self, char *key) {
  int id=0;
  for(id=0;id<self->n;id++) if(!strcmp((self->type[id]).key,key)) return id;
  return -1;
}

DOUBLE* cqlsdataCenter_DBL_key(DATA_CENTER self,char *key) {
  return cqlsdata_DBL(self->curObj,cqlsdataCenter_idData(self,key));
}

int* cqlsdataCenter_INT_key(DATA_CENTER self, char *key) {
  return cqlsdata_INT(self->curObj,cqlsdataCenter_idData(self,key));
}

int cqlsdataCenter_size_key(DATA_CENTER self, char *key) {
  return cqlsdataCenter_size(self,cqlsdataCenter_idData(self,key));
}

void* cqlsdata_get(DATA self,int id) {
  return self->data[id];
}

DOUBLE* cqlsdata_DBL(DATA self,int id) {
  return (DOUBLE*)(self->data[id]);
}

int* cqlsdata_INT(DATA self,int id) {
  return (int*)(self->data[id]);
}

DATA_USER cqlsdataUser_new(DATA_CENTER dc) {
  DATA_USER self;

  self=(DATA_USER)Calloc(1,ST_DATA_USER);
  self->nbData=0;
  self->idData=NULL;
  self->dc=dc;
  return self;
}

void cqlsdataUser_free(DATA_USER self) {
  if(self!=(DATA_USER)NULL) {
    Free(self->idData);
    Free(self);
  }
}

int cqlsdataUser_declare(DATA_USER self,int size,DATA_TYPE type, char *key) {
  int id;
  id=self->nbData;
  (self->nbData)++;
  self->idData=(int*)Realloc(self->idData,self->nbData,int);
  return (self->idData[id]=cqlsdataCenter_declare(self->dc,size,type,key));
}

DOUBLE* cqlsdataUser_DBL(DATA_USER self,int id) {
  return cqlsdataCenter_DBL(self->dc,self->idData[id]);
}

int* cqlsdataUser_INT(DATA_USER self, int id) {
  return cqlsdataCenter_INT(self->dc,self->idData[id]);
}

int cqlsdataUser_idData(DATA_USER self, char *key) {
  int id=0;
  for(id=0;id<self->nbData;id++) if(!strcmp((self->dc->type[self->idData[id]]).key,key)) return id;
  return -1;
}

DOUBLE* cqlsdataUser_DBL_key(DATA_USER self,char *key) {
  return cqlsdata_DBL(self->dc->curObj,cqlsdataUser_idData(self,key));
}

int* cqlsdataUser_INT_key(DATA_USER self, char *key) {
  return cqlsdata_INT(self->dc->curObj,cqlsdataUser_idData(self,key));
}

//cqlsR side!
/*int cqlsR_declare(CQLS_R self,int size,int type, char *key) {
  return cqlsdataCenter_declare(self->dc,size,type,key);
}

void cqlsR_newData(CQLS_R self) {
  cqlsdataCenter_newData(self->dc);
}

void cqlsR_freeData(CQLS_R self) {
  cqlsdataCenter_freeData(self->dc);
}

DOUBLE* cqlsR_DBL(CQLS_R self,int id) {
  return cqlsdataCenter_DBL(self->dc,id);
}

int* cqlsR_INT(CQLS_R self, int id) {
  return cqlsdataCenter_INT(self->dc,id);
}*/
