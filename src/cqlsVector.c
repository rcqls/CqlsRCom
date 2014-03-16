#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cqlsR.h"
#include "eblist.h"
#include "cqlsVector.h"
//#include "ebcalcul.h"

#define debugModeR_OFF

#ifndef Pb_M
#define Pb_M(vect,id) if(vect==NULL) {Rprintf("%d,Probleme de memoire\n",id); exit(0);}
#endif

//CQLS_VECTOR
void cqlsVector_free(CQLS_VECTOR self) {
  if (self!=(CQLS_VECTOR)NULL) {
    Free(self->sizeBlockVector);
    Free(self->keyBlockVector);
    Free(self->ansCache);
    Free(self->getteR);
    Free(self->setteR);
    Free(self);
  }
}

CQLS_VECTOR cqlsVector_new() {
  CQLS_VECTOR self;
  
  self=(CQLS_VECTOR)Calloc(1,ST_CQLS_VECTOR);
  self->nbVector=0;
  self->nbBlockVector=0;
  self->sizeBlockVector=NULL;
  self->keyBlockVector=NULL;
  self->ansCache=(DOUBLE*)NULL;
  self->getteR=(SEXP*)NULL;
  self->setteR=(SEXP*)NULL;
  self->envR=(CQLS_R)NULL;//to initialize!
  return self;
}

int cqlsVector_realloc(CQLS_VECTOR self, int nbBlockVector) {
  int oldNbBlockVector=self->nbBlockVector;

  self->nbBlockVector += nbBlockVector;
  self->keyBlockVector=(char**)Realloc(self->keyBlockVector,self->nbBlockVector,char*);Pb_M(self->keyBlockVector,-1);
  self->sizeBlockVector=(int*)Realloc(self->sizeBlockVector,self->nbBlockVector,int);Pb_M(self->sizeBlockVector,-2);
  self->getteR=(SEXP*)Realloc(self->getteR,self->nbBlockVector,SEXP);Pb_M(self->getteR,-10);
  self->setteR=(SEXP*)Realloc(self->setteR,self->nbBlockVector,SEXP);Pb_M(self->setteR,-11);
  //all the components of the vector!
  self->ansCache=(DOUBLE*)Realloc(self->ansCache,self->nbVector,DOUBLE);Pb_M(self->ansCache,-20);
  return oldNbBlockVector;
}


SEXP cqlsVector_eval_cmd(CQLS_R renv, SEXP cmdR) {
  SEXP ans;
  int status;

  PROTECT(ans = R_tryEval(VECTOR_ELT(cmdR, 0),renv->env, &status));
  if(status) {
    //fRprintf(stderr, "Caught another error calling sqrt()\n");
    ans=R_NilValue;
    fflush(stderr);
  }
  UNPROTECT(1);
  return ans;
}

void cqlsVector_add_block(CQLS_VECTOR self, int size, char* key, char* formulaR) {
  int i;
  char* cmdR;

  self->nbVector += size;
  i=cqlsVector_realloc(self,1);
  self->keyBlockVector[i]=key;
  self->sizeBlockVector[i]=size;
  R_PreserveObject(self->getteR[i]=CQLSR_parse1(key));
  if(self->getteR[i]==R_NilValue) Rprintf("Error parsing in getteR[%d]:%s\n",i,key);
  cmdR=(char*)Calloc(strlen(key)+strlen(formulaR)+3,char);
  sprintf(cmdR,"%s<-%s",key,formulaR);
#ifdef debugModeR
  Rprintf("cmdR: %s\n",cmdR);
#endif
  R_PreserveObject(self->setteR[i]=CQLSR_parse1(cmdR));
  if(self->setteR[i]==R_NilValue) Rprintf("Error parsing in setteR[%d]:%s\n",i,cmdR);
  self->ansCache[i]=0.0; //initialisation to 0
}

void cqlsVector_append(CQLS_VECTOR self, CQLS_VECTOR vect) {
  int i,ii;

  ii=cqlsVector_realloc(self,vect->nbBlockVector);
  for(i=0;i<vect->nbBlockVector;i++,ii++) {
    self->keyBlockVector[ii]=vect->keyBlockVector[i];
    self->sizeBlockVector[ii]=vect->sizeBlockVector[i];
    self->ansCache[ii]=vect->ansCache[i];
    self->getteR[ii]=vect->getteR[i];
    self->setteR[ii]=vect->setteR[i];
  }
  self->nbVector += vect->nbVector;
}

//get or eval from R and create a C vector
//sum stuff!
DOUBLE*  cqlsVector_newVector(CQLS_VECTOR self) {
  DOUBLE* vector;
  vector=(DOUBLE*)Calloc(self->nbVector,DOUBLE);Pb_M(vector,-100);
  cqlsVector_initVector(self,vector);
  return vector;
}

DOUBLE*  cqlsVector_copyVector(CQLS_VECTOR self) {
  DOUBLE* vector;
  int i;

  vector=(DOUBLE*)Calloc(self->nbVector,DOUBLE);Pb_M(vector,-200);
  for(i=0;i<self->nbVector;i++) vector[i]=self->ansCache[i];
  return vector;
}



void  cqlsVector_initVector(CQLS_VECTOR self,DOUBLE* vector) {
  int i;

  for(i=0;i<self->nbVector;i++) vector[i]=(DOUBLE)0.0;
}

void  cqlsVector_initVector_fromCache(CQLS_VECTOR self,DOUBLE* vector) {
  int i;

  for(i=0;i<self->nbVector;i++) vector[i]=(DOUBLE)self->ansCache[i];
}

short cqlsVector_same_vectors(CQLS_VECTOR self,DOUBLE* vector,DOUBLE* vector2) {
  int i;
  short res=1;
  
  //Rprintf("equality between vectors:\n");
  //cqlsVector_print(self,vector);cqlsVector_print(self,vector2);
  for(i=0;i<self->nbVector;i++) if(vector[i]!=vector2[i]) {res=0;break;}
  //Rprintf("->%d\n",res);
  return res;
}

void cqlsVector_vector_increased_by_weighted_vector(CQLS_VECTOR self, DOUBLE* res, DOUBLE* vector,DOUBLE coeff) {
  int i;
  
  for(i=0;i<self->nbVector;i++) res[i] += coeff*(vector[i]);
}

void  cqlsVector_initCache(CQLS_VECTOR self) {
   cqlsVector_initVector(self,self->ansCache);
}


//no save, only eval (used for calculation)!
void cqlsVector_eval(CQLS_VECTOR self,CQLS_R envR) {
  int i;
  
  for(i=0;i<self->nbBlockVector;i++) {
    //id by key since not so often used!!
    //res=findVar(install(self->keyBlockVector[i]),CQLSR->env);
    cqlsVector_eval_cmd(envR,self->setteR[i]);
  }
}

//the only difference between the two following functions is that 
// the first one update ansCache after the setteR is applied when
// the second one fetch the value from R with the getteR

void cqlsVector_from_envR_to_vector_after_update(CQLS_VECTOR self,DOUBLE* vector,CQLS_R envR) {
  int i,j,ii;
  SEXP res;

  for(i=0,ii=0;i<self->nbBlockVector;i++) {
    //id by key since not so often used!!
    //res=findVar(install(self->keyBlockVector[i]),CQLSR->env);
    PROTECT(res=cqlsVector_eval_cmd(envR,self->setteR[i]));
    for(j=0;j<self->sizeBlockVector[i];j++,ii++) {
      vector[ii]=cqlsSEXP_DBL(res,j);
    }
    UNPROTECT(1);
  }
}

void cqlsVector_from_envR_to_vector(CQLS_VECTOR self,DOUBLE* vector,CQLS_R envR) {
  int i,ii,j;
  SEXP res;

  for(i=0,ii=0;i<self->nbBlockVector;i++) {
    //id by key since not so often used!!
    //res=findVar(install(self->keyBlockVector[i]),CQLSR->env);
    PROTECT(res=cqlsVector_eval_cmd(envR,self->getteR[i]));
    for(j=0;j<self->sizeBlockVector[i];j++,ii++) {
      vector[ii]=cqlsSEXP_DBL(res,j);
    }
    UNPROTECT(1);
  }
}

DOUBLE* cqlsVector_from_envR_to_new_vector_after_update(CQLS_VECTOR self,CQLS_R envR) {
  DOUBLE* vector;
  vector=cqlsVector_newVector(self);
  cqlsVector_from_envR_to_vector_after_update(self,vector,envR);
  return vector;
}

void cqlsVector_vector_increased_by_weighted_updated_vector_from_envR(CQLS_VECTOR self,DOUBLE* res,DOUBLE coeff,CQLS_R envR) {
  DOUBLE* vector;

  vector=(DOUBLE*)Calloc(self->nbVector,DOUBLE);Pb_M(vector,-100);
  cqlsVector_initVector(self,vector);
  cqlsVector_from_envR_to_vector_after_update(self,vector,envR);//Rprintf("tmp=%d,self->ansCache=%p\n",tmp,self->ansCache);
  //Rprintf("tmp->");cqlsVector_print(self,tmp);
  cqlsVector_vector_increased_by_weighted_vector(self, res, vector,coeff);//Rprintf("coeff=%LF\n",coeff);
  //Rprintf("self->cache: ->");cqlsVector_print_cache(self);Rprintf("done!\n");
  Free(vector);
}

DOUBLE* cqlsVector_from_envR_to_new_vector(CQLS_VECTOR self,CQLS_R envR) {
  DOUBLE* vector;
  vector=cqlsVector_newVector(self);
  cqlsVector_from_envR_to_vector(self,vector,envR);
  return vector;
}

void cqlsVector_from_vector_to_envR(CQLS_VECTOR self, DOUBLE* vector,CQLS_R envR) {
  int i,j;
  
  for(i=0;i<self->nbBlockVector;i++) {
    cqlsR_DBL2SEXP(envR,self->keyBlockVector[i],vector,self->sizeBlockVector[i]);
    vector += self->sizeBlockVector[i];
  }
}

SEXP cqlsVector_vector_to_R_as_named_list(CQLS_VECTOR self, DOUBLE* vector) {
  //return a list of element vector 
  SEXP resR,nameR,tmpR;
  int i,ii,j;

  resR=R_NilValue;

  if(self!=(CQLS_VECTOR)NULL) {
    if(self->nbBlockVector==0) return R_NilValue;
    PROTECT(resR=allocVector(VECSXP,self->nbBlockVector));
    PROTECT(nameR=allocVector(STRSXP,self->nbBlockVector));
    for(i=0,ii=0;i<self->nbBlockVector;i++) {
      PROTECT(tmpR=allocVector(REALSXP,self->sizeBlockVector[i]));
      for(j=0;j<self->sizeBlockVector[i];j++,ii++) REAL(tmpR)[j]=vector[ii];
      SET_VECTOR_ELT(resR,i,tmpR);
      UNPROTECT(1);
      SET_STRING_ELT(nameR,i,mkChar(self->keyBlockVector[i]));
    }
    setAttrib(resR,R_NamesSymbol,nameR);
    UNPROTECT(2);  
  }
  return resR;
}

//cache
void cqlsVector_from_envR_to_cache_after_update(CQLS_VECTOR self,CQLS_R envR) {
  cqlsVector_from_envR_to_vector_after_update(self,self->ansCache,envR);
}

void cqlsVector_from_envR_to_cache(CQLS_VECTOR self,CQLS_R envR) {
  cqlsVector_from_envR_to_vector(self,self->ansCache,envR);
}

void cqlsVector_from_cache_to_envR(CQLS_VECTOR self, CQLS_R envR) {
  cqlsVector_from_vector_to_envR(self,self->ansCache,envR);  
}

SEXP cqlsVector_cache_to_R_as_named_list(CQLS_VECTOR self) {
  return cqlsVector_vector_to_R_as_named_list(self,self->ansCache);
}

//cache as a cumulative vector
void cqlsVector_cache_increased_by_weighted_updated_vector_from_envR(CQLS_VECTOR self,DOUBLE coeff,CQLS_R envR) {
  //Rprintf("ici\n");Rprintf("self->cache:"),cqlsVector_print_cache(self);
  //Rprintf("coeff=%LF,envR=%p\n",coeff,envR);
   cqlsVector_vector_increased_by_weighted_updated_vector_from_envR(self,self->ansCache,coeff,envR);
}

//show vector!
void cqlsVector_show(CQLS_VECTOR self, char *title) {
  int i;

  Rprintf("%s->nbVector=%d",title,self->nbVector);
  for(i=0;i<self->nbBlockVector;i++) Rprintf(",%s(%d)",self->keyBlockVector[i],self->sizeBlockVector[i]);
  Rprintf("\n");
}

void cqlsVector_print(CQLS_VECTOR self,DOUBLE* vector) {
  int i,j,ii;

  if(self->nbBlockVector==0) Rprintf("Empty cqls_vector!\n");
  else {
    for(i=0,ii=0;i<self->nbBlockVector;i++) for(j=0;j<self->sizeBlockVector[i];j++,ii++) Rprintf("%s[%d]=%LF\t",self->keyBlockVector[i],j+1,vector[ii]);Rprintf("\n");
  }
}


void cqlsVector_print_cache(CQLS_VECTOR self) {
  cqlsVector_print(self,self->ansCache);
}


// CQLS_VECTOR_LIST
CQLS_VECTOR_LIST cqlsVectorList_new(CQLS_VECTOR vector) {
  CQLS_VECTOR_LIST self;
  
  self=(CQLS_VECTOR_LIST)Calloc(1,ST_CQLS_VECTOR_LIST);Pb_M(self,1000);
  self->vector=vector;
  self->list=NULL;
  return self;
}

void cqlsVectorList_free(CQLS_VECTOR_LIST self) {
  if (self!=(CQLS_VECTOR_LIST)NULL) {
    cqlsVectorList_empty(self); 
    self->list=NULL;
    Free(self);
  }
}

void cqlsVectorList_empty(CQLS_VECTOR_LIST self) {
  DOUBLE* vector;
  //Do not do the job since the elements are not cleaned: eblist_vider(&(self->list)); 
  while(self->list!=NULL) {
    vector = (DOUBLE*) eblist_recup_tete(&self->list);
    //Rprintf("Vider Liste: %p\n",vector);
    Free(vector);
  }
}

void cqlsVectorList_insert(CQLS_VECTOR_LIST self,DOUBLE* vector) {
  //Rprintf("Ins VectorList: %p\n",vector);
  eblist_ins_tete(&(self->list),vector);
}

/*void cqlsVectorList_insert(CQLS_VECTOR_LIST self) {
  eblist_ins_tete(&(self->list),(DOUBLE*)(self->vector->ansCache));
}*/

void cqlsVectorList_print(CQLS_VECTOR_LIST self) {
  int i;
  PT_LIST pt;

  for(pt=self->list,i=1;pt!=NULL;pt=pt->pt_next,i++) {  
    //Rprintf("pt:%d\n",pt);  
    Rprintf("%d: ",i);cqlsVector_print(self->vector,(DOUBLE*)pt->pt_cur);
  }
}

//R development

#include <R.h>
#include <Rinternals.h>
#include <R_ext/PrtUtil.h>

SEXP cqlsVector_Rnames(CQLS_VECTOR self) {
  int i,j,ii;
  SEXP names;

  PROTECT(names=allocVector(STRSXP,self->nbVector));
  for(i=0,ii=0;i<self->nbBlockVector;i++) for(j=0;j<self->sizeBlockVector[i];j++,ii++) 
    SET_STRING_ELT(names,ii,mkChar(self->keyBlockVector[i]));
  UNPROTECT(1);
  return names;
}

SEXP cqlsVector_vector_array_to_R_as_dataframe(CQLS_VECTOR vector,DOUBLE** vectorAry,int arySize,int* weight) {
  int k,i;
  SEXP resR,*tmpR,tmpR2,rownameR;  

//Rprintf("cqlsVector_vector_array_to_R_as_dataframe\n");
  PROTECT(resR=allocVector(VECSXP,vector->nbVector));//resR<-list()
  tmpR=(SEXP*)Calloc(vector->nbVector,SEXP);
  PROTECT(rownameR = allocVector(INTSXP, arySize));
  for(i=0;i<vector->nbVector;i++) PROTECT(tmpR[i]=allocVector(REALSXP,arySize));

  for(k=0;k<arySize;k++) {    
//Rprintf("k=%d,%LF\n",k,vectorAry[k][0]);
    for(i=0;i<vector->nbVector;i++) REAL(tmpR[i])[k] = vectorAry[k][i];  
    INTEGER(rownameR)[k] = k+1;
  }

  for(i=0;i<vector->nbVector;i++) SET_VECTOR_ELT(resR,i,tmpR[i]);

  setAttrib(resR, R_NamesSymbol,cqlsVector_Rnames(vector));
 
  PROTECT(tmpR2 = allocVector(STRSXP, 1));
  SET_STRING_ELT(tmpR2, 0, mkChar("data.frame"));
  classgets(resR, tmpR2);
  setAttrib(resR, R_RowNamesSymbol, rownameR);
  UNPROTECT(1);
  UNPROTECT(2 + vector->nbVector);
  Free(tmpR);
  return resR;
}


SEXP cqlsVectorList_to_R_as_dataframe(CQLS_VECTOR_LIST self) {
  PT_LIST pt;
  int k,i,j,ii,cpt;
  SEXP resR,*tmpR,tmpR2,rownameR;  
  CQLS_VECTOR vector;
  DOUBLE *res; 

  vector=self->vector;
  cpt=eblist_length(self->list);
  PROTECT(resR=allocVector(VECSXP,vector->nbVector));//resR<-list()
  tmpR=(SEXP*)Calloc(vector->nbVector,SEXP);
  PROTECT(rownameR = allocVector(INTSXP, cpt));
  for(i=0;i<vector->nbVector;i++) PROTECT(tmpR[i]=allocVector(REALSXP,cpt));

  for(pt=self->list,k=0;pt!=NULL;pt=pt->pt_next,k++) {
    res=(DOUBLE*)pt->pt_cur;    
//Rprintf("k=%d,%LF\n",k,res[0]);
    for(i=0;i<vector->nbVector;i++) REAL(tmpR[i])[k] = res[i];  
    INTEGER(rownameR)[k] = k+1;
  }

  for(i=0;i<vector->nbVector;i++) SET_VECTOR_ELT(resR,i,tmpR[i]);

  setAttrib(resR, R_NamesSymbol,cqlsVector_Rnames(vector));
 
  PROTECT(tmpR2 = allocVector(STRSXP, 1));
  SET_STRING_ELT(tmpR2, 0, mkChar("data.frame"));
  classgets(resR, tmpR2);
  setAttrib(resR, R_RowNamesSymbol, rownameR);
  UNPROTECT(1);
  UNPROTECT(2 + vector->nbVector);
  Free(tmpR);
  return resR;
}

