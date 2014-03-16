#include <stdio.h>
#include <string.h>
#include "cqlsR.h"

//SEXP util!

DOUBLE cqlsSEXP_DBL(SEXP ans,int i) {
  switch(TYPEOF(ans)) {
    case REALSXP: return (DOUBLE)REAL(ans)[i];
    case INTSXP:
      case LGLSXP: return (DOUBLE)INTEGER(ans)[i];
  }
  return (DOUBLE)0.0;//Dangerous!!!->no meaning
}

int cqlsSEXP_INT(SEXP ans,int i) {
  switch(TYPEOF(ans)) {
    case REALSXP: return (int)REAL(ans)[i];
    case INTSXP:
      case LGLSXP: return (int)INTEGER(ans)[i];
  }
  return 0;//Dangerous!!!-> no meaning!!!
}

//DATA_CENTER
void cqlsSEXP_setAllVars_List(SEXP list,DATA_CENTER dc) {
  ST_DATA_STRUCT type;
  SEXP tmp,name;
  int i,id;
  
  PROTECT(name=allocVector(STRSXP,dc->n));
  for(id=0;id<dc->n;id++) {
    type=dc->type[id];
    SET_STRING_ELT(name,id,mkChar(type.key));
    switch(type.type) {
      case DATA_DBL:
	      PROTECT(tmp=allocVector(REALSXP,type.size));
	      for(i=0;i<type.size;i++) {
	        REAL(tmp)[i]=DC_DBL(dc,id)[i];
	      }
	      SET_VECTOR_ELT(list,id,tmp);
	      UNPROTECT(1);
	      break;
      case DATA_INT:
	      PROTECT(tmp=allocVector(INTSXP,type.size));
	      for(i=0;i<type.size;i++) {
	        INTEGER(tmp)[i]=DC_INT(dc,id)[i];
	      }
	      SET_VECTOR_ELT(list,id,tmp);
	      UNPROTECT(1);
	      break;
    }
  }
  setAttrib(list,R_NamesSymbol,name);
  UNPROTECT(1);
}

void cqlsSEXP_setAllVarsNA_List(SEXP list,DATA_CENTER dc) {
  ST_DATA_STRUCT type;
  SEXP tmp,name;
  int i,id;

  for(id=0;id<dc->n;id++) {
    type=dc->type[id];
    switch(type.type) {
      case DATA_DBL:
	PROTECT(tmp=coerceVector(VECTOR_ELT(list,id),REALSXP));
	for(i=0;i<type.size;i++) {
	  REAL(tmp)[i]=NA_REAL;
	}
	break;
      case DATA_INT:
	PROTECT(tmp=coerceVector(VECTOR_ELT(list,id),INTSXP));
	for(i=0;i<type.size;i++) {
	  INTEGER(tmp)[i]=NA_INTEGER;
	}
	break;
    }
    UNPROTECT(1);
  }
}

void cqlsSEXP_getAllVars_List(SEXP list,DATA_CENTER dc) {
  ST_DATA_STRUCT type;
  SEXP tmp,name;
  int i,id;

  for(id=0;id<dc->n;id++) {
    type=dc->type[id];
    switch(type.type) {
      case DATA_DBL:
	PROTECT(tmp=coerceVector(VECTOR_ELT(list,id),REALSXP));
	for(i=0;i<type.size;i++) {
	  DC_DBL(dc,id)[i]=(DOUBLE)(REAL(tmp)[i]);
	}
	break;
      case DATA_INT:
	PROTECT(tmp=coerceVector(VECTOR_ELT(list,id),INTSXP));
	for(i=0;i<type.size;i++) {
	  DC_INT(dc,id)[i]=INTEGER(tmp)[i];
	}
	break;
    }
    UNPROTECT(1);
  }
}

//TODO: FAIRE COMME cqlsSEXP_setAllVars_List!
void cqlsSEXP_setAllVars_List_key(SEXP list,DATA_CENTER dc) {
  ST_DATA_STRUCT type;
  SEXP tmp,name;
  int i,id,k;
  char *key;

  name=getAttrib(list,R_NamesSymbol);
  for(k=0;k<length(name);k++) {
    key=CHAR(STRING_ELT(name,k));
    id=DC_idData(dc,key);
    //printf("key=%s,id=%d\n",key,id);
    type=dc->type[id];
    switch(type.type) {
      case DATA_DBL:
	PROTECT(tmp=coerceVector(VECTOR_ELT(list,k),REALSXP));
	for(i=0;i<type.size;i++) {
	  REAL(tmp)[i]=DC_DBL(dc,id)[i];
	}
	break;
      case DATA_INT:
	PROTECT(tmp=coerceVector(VECTOR_ELT(list,k),INTSXP));
	for(i=0;i<type.size;i++) {
	  INTEGER(tmp)[i]=DC_INT(dc,id)[i];
	}
	break;
    }
    UNPROTECT(1);
  }
}

void cqlsSEXP_getAllVars_List_key(SEXP list,DATA_CENTER dc) {
  ST_DATA_STRUCT type;
  SEXP tmp,name;
  int i,id,k;
  char *key;

  name=getAttrib(list,R_NamesSymbol);
  for(k=0;k<length(name);k++) {
    key=CHAR(STRING_ELT(name,k));
    id=DC_idData(dc,key);
    //printf("key=%s,id=%d\n",key,id);
    type=dc->type[id];
    switch(type.type) {
      case DATA_DBL:
	PROTECT(tmp=coerceVector(VECTOR_ELT(list,k),REALSXP));
	for(i=0;i<type.size;i++) {
	  DC_DBL(dc,id)[i]=(DOUBLE)(REAL(tmp)[i]);
	}
	break;
      case DATA_INT:
	PROTECT(tmp=coerceVector(VECTOR_ELT(list,k),INTSXP));
	for(i=0;i<type.size;i++) {
	  DC_INT(dc,id)[i]=INTEGER(tmp)[i];
	}
	break;
    }
    UNPROTECT(1);
  }
}

void cqlsSEXP_setAllVars_MDF(SEXP mdf,int i,DATA_CENTER dc) {
  ST_DATA_STRUCT type;
  SEXP tmp,name;
  int j,id,nr;
  char *key;
  
  name=getAttrib(mdf,R_NamesSymbol);
  for(id=0;id<dc->n;id++) {
    type=dc->type[id];
    switch(type.type) {
      case DATA_DBL:
	PROTECT(tmp=coerceVector(VECTOR_ELT(mdf,id),REALSXP));
	nr=length(tmp)/type.size;
	for(j=0;j<type.size;j++) {
	  REAL(tmp)[i+nr*j]=DC_DBL(dc,id)[j];
	}
	break;
      case DATA_INT:
	PROTECT(tmp=coerceVector(VECTOR_ELT(mdf,id),INTSXP));
	nr=length(tmp)/type.size;
	for(j=0;j<type.size;j++) {
	  INTEGER(tmp)[i+nr*j]=DC_INT(dc,id)[j];
	}
	break;
    }
    UNPROTECT(1);
  }
}

void cqlsSEXP_setAllVarsNA_MDF(SEXP mdf,int i,DATA_CENTER dc) {
  ST_DATA_STRUCT type;
  SEXP tmp,name;
  int j,id,nr;
  char *key;
  
  name=getAttrib(mdf,R_NamesSymbol);
  for(id=0;id<dc->n;id++) {
    type=dc->type[id];
    switch(type.type) {
      case DATA_DBL:
	PROTECT(tmp=coerceVector(VECTOR_ELT(mdf,id),REALSXP));
	nr=length(tmp)/type.size;
	for(j=0;j<type.size;j++) {
	  REAL(tmp)[i+nr*j]=NA_REAL;
	}
	break;
      case DATA_INT:
	PROTECT(tmp=coerceVector(VECTOR_ELT(mdf,id),INTSXP));
	nr=length(tmp)/type.size;
	for(j=0;j<type.size;j++) {
	  INTEGER(tmp)[i+nr*j]=NA_INTEGER;
	}
	break;
    }
    UNPROTECT(1);
  }
}

void cqlsSEXP_getAllVars_MDF(SEXP mdf,int i,DATA_CENTER dc) {
  ST_DATA_STRUCT type;
  SEXP tmp,name;
  int j,id,nr;
  char *key;
  
  name=getAttrib(mdf,R_NamesSymbol);
  for(id=0;id<dc->n;id++) {
    type=dc->type[id];
    switch(type.type) {
      case DATA_DBL:
	PROTECT(tmp=coerceVector(VECTOR_ELT(mdf,id),REALSXP));
	nr=length(tmp)/type.size;
	for(j=0;j<type.size;j++) {
	  DC_DBL(dc,id)[j]=(DOUBLE)(REAL(tmp)[i+nr*j]);
	}
	break;
      case DATA_INT:
	PROTECT(tmp=coerceVector(VECTOR_ELT(mdf,id),INTSXP));
	nr=length(tmp)/type.size;
	for(j=0;j<type.size;j++) {
	  DC_INT(dc,id)[j]=INTEGER(tmp)[i+nr*j];
	}
	break;
    }
    UNPROTECT(1);
  }
}

void cqlsSEXP_setAllVars_MDF_key(SEXP mdf,int i,DATA_CENTER dc) {
  ST_DATA_STRUCT type;
  SEXP tmp,name;
  int j,id,k,nr;
  char *key;
  
  name=getAttrib(mdf,R_NamesSymbol);
  for(k=0;k<length(name);k++) {
    key=CHAR(STRING_ELT(name,k));
    id=DC_idData(dc,key);
    //printf("key=%s,id=%d\n",key,id);
    type=dc->type[id];
    switch(type.type) {
      case DATA_DBL:
	PROTECT(tmp=coerceVector(VECTOR_ELT(mdf,k),REALSXP));
	nr=length(tmp)/type.size;
	for(j=0;j<type.size;j++) {
	  REAL(tmp)[i+nr*j]=DC_DBL(dc,id)[j];
	}
	break;
      case DATA_INT:
	PROTECT(tmp=coerceVector(VECTOR_ELT(mdf,k),INTSXP));
	nr=length(tmp)/type.size;
	for(j=0;j<type.size;j++) {
	  INTEGER(tmp)[i+nr*j]=DC_INT(dc,id)[j];
	}
	break;
    }
    UNPROTECT(1);
  }
}

void cqlsSEXP_getAllVars_MDF_key(SEXP mdf,int i,DATA_CENTER dc) {
  ST_DATA_STRUCT type;
  SEXP tmp,name;
  int j,id,k,nr;
  char *key;
  
  name=getAttrib(mdf,R_NamesSymbol);
  for(k=0;k<length(name);k++) {
    key=CHAR(STRING_ELT(name,k));
    id=DC_idData(dc,key);
    //printf("key=%s,id=%d\n",key,id);
    type=dc->type[id];
    switch(type.type) {
      case DATA_DBL:
	PROTECT(tmp=coerceVector(VECTOR_ELT(mdf,k),REALSXP));
	nr=length(tmp)/type.size;
	for(j=0;j<type.size;j++) {
	  DC_DBL(dc,id)[j]=(DOUBLE)(REAL(tmp)[i+nr*j]);
	}
	break;
      case DATA_INT:
	PROTECT(tmp=coerceVector(VECTOR_ELT(mdf,k),INTSXP));
	nr=length(tmp)/type.size;
	for(j=0;j<type.size;j++) {
	  DC_INT(dc,id)[j]=INTEGER(tmp)[i+nr*j];
	}
	break;
    }
    UNPROTECT(1);
  }
}

//DATA_USER
void DU_cqlsSEXP_setAllVars_List(SEXP list,DATA_USER du) {
  ST_DATA_STRUCT type;
  SEXP tmp,name;
  int i,id;
  
  PROTECT(name=allocVector(STRSXP,du->nbData));
  for(id=0;id<du->nbData;id++) {
    type=DU_type(du,id);
    SET_STRING_ELT(name,id,mkChar(type.key));
    switch(type.type) {
      case DATA_DBL:
	PROTECT(tmp=allocVector(REALSXP,type.size));
	for(i=0;i<type.size;i++) {
	  REAL(tmp)[i]=DU_DBL(du,id)[i];
	}
	SET_VECTOR_ELT(list,id,tmp);
	UNPROTECT(1);
	break;
      case DATA_INT:
	PROTECT(tmp=allocVector(INTSXP,type.size));
	for(i=0;i<type.size;i++) {
	  INTEGER(tmp)[i]=DU_INT(du,id)[i];
	}
	SET_VECTOR_ELT(list,id,tmp);
	UNPROTECT(1);
	break;
    }
  }
  setAttrib(list,R_NamesSymbol,name);
  UNPROTECT(1);
}

void DU_cqlsSEXP_setAllVarsNA_List(SEXP list,DATA_USER du) {
  ST_DATA_STRUCT type;
  SEXP tmp,name;
  int i,id;

  for(id=0;id<du->nbData;id++) {
    type=DU_type(du,id);
    switch(type.type) {
      case DATA_DBL:
	PROTECT(tmp=coerceVector(VECTOR_ELT(list,id),REALSXP));
	for(i=0;i<type.size;i++) {
	  REAL(tmp)[i]=NA_REAL;
	}
	break;
      case DATA_INT:
	PROTECT(tmp=coerceVector(VECTOR_ELT(list,id),INTSXP));
	for(i=0;i<type.size;i++) {
	  INTEGER(tmp)[i]=NA_INTEGER;
	}
	break;
    }
    UNPROTECT(1);
  }
}

void DU_cqlsSEXP_getAllVars_List(SEXP list,DATA_USER du) {
  ST_DATA_STRUCT type;
  SEXP tmp,name;
  int i,id;

  for(id=0;id<du->nbData;id++) {
    type=DU_type(du,id);
    switch(type.type) {
      case DATA_DBL:
	PROTECT(tmp=coerceVector(VECTOR_ELT(list,id),REALSXP));
	for(i=0;i<type.size;i++) {
	  DU_DBL(du,id)[i]=(DOUBLE)(REAL(tmp)[i]);
	}
	break;
      case DATA_INT:
	PROTECT(tmp=coerceVector(VECTOR_ELT(list,id),INTSXP));
	for(i=0;i<type.size;i++) {
	  DU_INT(du,id)[i]=INTEGER(tmp)[i];
	}
	break;
    }
    UNPROTECT(1);
  }
}

//TODO: FAIRE COMME cqlsSEXP_setAllVars_List!
void DU_cqlsSEXP_setAllVars_List_key(SEXP list,DATA_USER du) {
  ST_DATA_STRUCT type;
  SEXP tmp,name;
  int i,id,k;
  char *key;

  name=getAttrib(list,R_NamesSymbol);
  for(k=0;k<length(name);k++) {
    key=CHAR(STRING_ELT(name,k));
    id=DU_idData(du,key);
    //printf("key=%s,id=%d\n",key,id);
    type=DU_type(du,id);
    switch(type.type) {
      case DATA_DBL:
	PROTECT(tmp=coerceVector(VECTOR_ELT(list,k),REALSXP));
	for(i=0;i<type.size;i++) {
	  REAL(tmp)[i]=DU_DBL(du,id)[i];
	}
	break;
      case DATA_INT:
	PROTECT(tmp=coerceVector(VECTOR_ELT(list,k),INTSXP));
	for(i=0;i<type.size;i++) {
	  INTEGER(tmp)[i]=DU_INT(du,id)[i];
	}
	break;
    }
    UNPROTECT(1);
  }
}

void DU_cqlsSEXP_getAllVars_List_key(SEXP list,DATA_USER du) {
  ST_DATA_STRUCT type;
  SEXP tmp,name;
  int i,id,k;
  char *key;

  name=getAttrib(list,R_NamesSymbol);
  for(k=0;k<length(name);k++) {
    key=CHAR(STRING_ELT(name,k));
    id=DU_idData(du,key);
    //printf("key=%s,id=%d\n",key,id);
    type=DU_type(du,id);
    switch(type.type) {
      case DATA_DBL:
	PROTECT(tmp=coerceVector(VECTOR_ELT(list,k),REALSXP));
	for(i=0;i<type.size;i++) {
	  DU_DBL(du,id)[i]=(DOUBLE)(REAL(tmp)[i]);
	}
	break;
      case DATA_INT:
	PROTECT(tmp=coerceVector(VECTOR_ELT(list,k),INTSXP));
	for(i=0;i<type.size;i++) {
	  DU_INT(du,id)[i]=INTEGER(tmp)[i];
	}
	break;
    }
    UNPROTECT(1);
  }
}

void DU_cqlsSEXP_setAllVars_MDF(SEXP mdf,int i,DATA_USER du) {
  ST_DATA_STRUCT type;
  SEXP tmp,name;
  int j,id,nr;
  char *key;
  
  name=getAttrib(mdf,R_NamesSymbol);
  for(id=0;id<du->nbData;id++) {
    type=DU_type(du,id);
    switch(type.type) {
      case DATA_DBL:
	PROTECT(tmp=coerceVector(VECTOR_ELT(mdf,id),REALSXP));
	nr=length(tmp)/type.size;
	for(j=0;j<type.size;j++) {
	  REAL(tmp)[i+nr*j]=DU_DBL(du,id)[j];
	}
	break;
      case DATA_INT:
	PROTECT(tmp=coerceVector(VECTOR_ELT(mdf,id),INTSXP));
	nr=length(tmp)/type.size;
	for(j=0;j<type.size;j++) {
	  INTEGER(tmp)[i+nr*j]=DU_INT(du,id)[j];
	}
	break;
    }
    UNPROTECT(1);
  }
}

void DU_cqlsSEXP_setAllVarsNA_MDF(SEXP mdf,int i,DATA_USER du) {
  ST_DATA_STRUCT type;
  SEXP tmp,name;
  int j,id,nr;
  char *key;
  
  name=getAttrib(mdf,R_NamesSymbol);
  for(id=0;id<du->nbData;id++) {
    type=DU_type(du,id);
    switch(type.type) {
      case DATA_DBL:
	PROTECT(tmp=coerceVector(VECTOR_ELT(mdf,id),REALSXP));
	nr=length(tmp)/type.size;
	for(j=0;j<type.size;j++) {
	  REAL(tmp)[i+nr*j]=NA_REAL;
	}
	break;
      case DATA_INT:
	PROTECT(tmp=coerceVector(VECTOR_ELT(mdf,id),INTSXP));
	nr=length(tmp)/type.size;
	for(j=0;j<type.size;j++) {
	  INTEGER(tmp)[i+nr*j]=NA_INTEGER;
	}
	break;
    }
    UNPROTECT(1);
  }
}

void DU_cqlsSEXP_getAllVars_MDF(SEXP mdf,int i,DATA_USER du) {
  ST_DATA_STRUCT type;
  SEXP tmp,name;
  int j,id,nr;
  char *key;
  
  name=getAttrib(mdf,R_NamesSymbol);
  for(id=0;id<du->nbData;id++) {
    type=DU_type(du,id);
    switch(type.type) {
      case DATA_DBL:
	PROTECT(tmp=coerceVector(VECTOR_ELT(mdf,id),REALSXP));
	nr=length(tmp)/type.size;
	for(j=0;j<type.size;j++) {
	  DU_DBL(du,id)[j]=(DOUBLE)(REAL(tmp)[i+nr*j]);
	}
	break;
      case DATA_INT:
	PROTECT(tmp=coerceVector(VECTOR_ELT(mdf,id),INTSXP));
	nr=length(tmp)/type.size;
	for(j=0;j<type.size;j++) {
	  DU_INT(du,id)[j]=INTEGER(tmp)[i+nr*j];
	}
	break;
    }
    UNPROTECT(1);
  }
}

void DU_cqlsSEXP_setAllVars_MDF_key(SEXP mdf,int i,DATA_USER du) {
  ST_DATA_STRUCT type;
  SEXP tmp,name;
  int j,id,k,nr;
  char *key;
  
  name=getAttrib(mdf,R_NamesSymbol);
  for(k=0;k<length(name);k++) {
    key=CHAR(STRING_ELT(name,k));
    id=DU_idData(du,key);
    //printf("key=%s,id=%d\n",key,id);
    type=DU_type(du,id);
    switch(type.type) {
      case DATA_DBL:
	PROTECT(tmp=coerceVector(VECTOR_ELT(mdf,k),REALSXP));
	nr=length(tmp)/type.size;
	for(j=0;j<type.size;j++) {
	  REAL(tmp)[i+nr*j]=DU_DBL(du,id)[j];
	}
	break;
      case DATA_INT:
	PROTECT(tmp=coerceVector(VECTOR_ELT(mdf,k),INTSXP));
	nr=length(tmp)/type.size;
	for(j=0;j<type.size;j++) {
	  INTEGER(tmp)[i+nr*j]=DU_INT(du,id)[j];
	}
	break;
    }
    UNPROTECT(1);
  }
}

void DU_cqlsSEXP_getAllVars_MDF_key(SEXP mdf,int i,DATA_USER du) {
  ST_DATA_STRUCT type;
  SEXP tmp,name;
  int j,id,k,nr;
  char *key;
  
  name=getAttrib(mdf,R_NamesSymbol);
  for(k=0;k<length(name);k++) {
    key=CHAR(STRING_ELT(name,k));
    id=DU_idData(du,key);
    //printf("key=%s,id=%d\n",key,id);
    type=DU_type(du,id);
    switch(type.type) {
      case DATA_DBL:
	PROTECT(tmp=coerceVector(VECTOR_ELT(mdf,k),REALSXP));
	nr=length(tmp)/type.size;
	for(j=0;j<type.size;j++) {
	  DU_DBL(du,id)[j]=(DOUBLE)(REAL(tmp)[i+nr*j]);
	}
	break;
      case DATA_INT:
	PROTECT(tmp=coerceVector(VECTOR_ELT(mdf,k),INTSXP));
	nr=length(tmp)/type.size;
	for(j=0;j<type.size;j++) {
	  DU_INT(du,id)[j]=INTEGER(tmp)[i+nr*j];
	}
	break;
    }
    UNPROTECT(1);
  }
}

//Global R Session! Only one instance!
void CQLSR_new() {
  char **argv;
  int argc;
  
  CQLSR=(CQLS_RSESSION)calloc(1,sizeof(ST_CQLS_RSESSION));
  //default parameters!
  //argc=4;
  //argv=(char**)Calloc(argc,char*);
  //argv[0]=(char*)Calloc(6,char);sprintf(argv[0],"REmbed");
  //argv[1]=(char*)Calloc(6,char);sprintf(argv[1],"--save");
  //argv[2]=(char*)Calloc(7,char);sprintf(argv[2],"--slave");
  //argv[3]=(char*)Calloc(7,char);sprintf(argv[3],"--quiet");
  //printf("argv=%x\n",argv);
  //CQLSR->argc=argc;
  //CQLSR->argv=argv;
  //CQLSR->env have to be set after R is started!
}


void CQLSR_start() {
  if(CQLSR==NULL) {
    CQLSR_new();
    CQLSR_init();
  }
}

//void CQLSR_within_R(int mode) {
//  CQLSR_WITHIN_R=mode;
//}

//void CQLSR_args(int argc,char* *argv) {
//  CQLSR->argc=argc;
//  CQLSR->argv=argv;
//}

int CQLSR_init() {
  //printf("R STARTED with CQLS_WITHIN_R=%d\n",CQLSR_WITHIN_R);
  //if(!CQLSR_WITHIN_R) {
    //Rf_initEmbeddedR(CQLSR->argc,CQLSR->argv);
  //};
  R_PreserveObject(CQLSR->env=R_GlobalEnv);
  return 1;
}

SEXP CQLSR_parse1(char *cmdString) {
  SEXP cmd;
  ParseStatus status;

  SEXP text, ans=R_NilValue /* -Wall */;

  PROTECT(text=allocVector(STRSXP, 1));
  SET_STRING_ELT(text, 0, mkChar(cmdString));
  PROTECT(cmd=RR_ParseVector(text, -1, &status));
  UNPROTECT(1);
  if (status != PARSE_OK) {
    Rprintf("Parsing error (status=%d) in:\n",status);
    return R_NilValue;
  }
  UNPROTECT(1);
  return cmd;
}

SEXP CQLSR_eval1(char *cmdString) {
  return cqlsREnv_eval1(CQLSR,cmdString);
}

SEXP CQLSR_DATA2SEXP(char *key,void *data,DATA_TYPE type,int size) {
  return cqlsREnv_DATA2SEXP(CQLSR,key,data,type,size);
}

SEXP CQLSR_DBL2SEXP(char *key,void *data,int size) {
  return cqlsREnv_DBL2SEXP(CQLSR,key,data,size);
}

SEXP CQLSR_INT2SEXP(char *key,void *data,int size) {
  return cqlsREnv_INT2SEXP(CQLSR,key,data,size);
}

void CQLSR_setVar(DATA_CENTER dc,int id) {
  cqlsREnv_setVar(CQLSR,dc,id);
}

void CQLSR_getVar(DATA_CENTER dc,int id) {
  cqlsREnv_getVar(CQLSR,dc,id);
}

void CQLSR_setVar_key(DATA_CENTER dc,char *key) {
  cqlsREnv_setVar_key(CQLSR,dc,key);
}

void CQLSR_getVar_key(DATA_CENTER dc,char *key) {
  cqlsREnv_getVar_key(CQLSR,dc,key);
}

void CQLSR_setAllVars_List(char *listName,DATA_CENTER dc) {
  cqlsREnv_setAllVars_List(CQLSR,listName,dc);
}

void CQLSR_getAllVars_List(char *listName,DATA_CENTER dc) {
  cqlsREnv_getAllVars_List(CQLSR,listName,dc);
}

void CQLSR_getAllVars_List_key(char *listName,DATA_CENTER dc) {
  cqlsREnv_getAllVars_List_key(CQLSR,listName,dc);
}

void CQLSR_getAllVars_MDF_key(char *listName,int i,DATA_CENTER dc) {
  cqlsREnv_getAllVars_MDF_key(CQLSR,listName,i,dc);
}

//eval in Environment!
void cqlsREnv_setEnv(CQLS_RENV renv, char *envName) {
  char *cmd;

  if(envName) {
    if(findVar(install(envName),CQLSR->env)==R_UnboundValue) {
      cmd=(char*)Calloc(strlen(envName)+12,char);
      sprintf(cmd,"%s<-new.env()",envName);
      CQLSR_eval1(cmd);
    }
    R_PreserveObject(renv->env=CQLSR_eval1(envName));
  } else R_PreserveObject(renv->env=CQLSR->env);
  renv->envName=envName;
}

SEXP cqlsREnv_eval1(CQLS_RENV renv, char *cmdString) {
  int status;
  SEXP cmd,ans;

  PROTECT(cmd=(SEXP)CQLSR_parse1(cmdString));
  PROTECT(ans = R_tryEval(VECTOR_ELT(cmd, 0),renv->env, &status));
  if(status) {
    //fprintf(stderr, "Caught another error calling sqrt()\n");
    fflush(stderr);
    return R_NilValue;
  }
  UNPROTECT(2);
  return ans;
}

SEXP cqlsREnv_DATA2SEXP(CQLS_RENV renv, char *key,void *data,DATA_TYPE type,int size) {
  int i;
  SEXP tmp;
  
  switch(type) {
    case DATA_DBL:
      PROTECT(tmp=allocVector(REALSXP,size));
      for(i=0;i<size;i++) {
	REAL(tmp)[i]=((DOUBLE*)data)[i];
      }
      break;
    case DATA_INT:
      PROTECT(tmp=allocVector(INTSXP,size));
      for(i=0;i<size;i++) {
	INTEGER(tmp)[i]=((int*)data)[i];
      }
      break;
  }
  defineVar(install(key),tmp,renv->env);
  UNPROTECT(1);
  return tmp;
}

SEXP cqlsREnv_DBL2SEXP(CQLS_RENV renv, char *key,DOUBLE *data,int size) {
  int i;
  SEXP tmp;
  
  PROTECT(tmp=allocVector(REALSXP,size));
  for(i=0;i<size;i++) {
    REAL(tmp)[i]=((DOUBLE*)data)[i];
  }
  defineVar(install(key),tmp,renv->env);
  UNPROTECT(1);
  return tmp;
}

SEXP cqlsREnv_INT2SEXP(CQLS_RENV renv, char *key,int *data,int size) {
  int i;
  SEXP tmp;
  
  PROTECT(tmp=allocVector(INTSXP,size));
  for(i=0;i<size;i++) {
    //Rprintf("data[%d]=%d\n",i,((int*)data)[i]);
    INTEGER(tmp)[i]=((int*)data)[i];
  }
  defineVar(install(key),tmp,renv->env);
  UNPROTECT(1);
  return tmp;
}


void cqlsREnv_setVar(CQLS_RENV renv,DATA_CENTER dc,int id) {
  ST_DATA_STRUCT type=dc->type[id];
  SEXP tmp;
  int i;
  
  switch(type.type) {
    case DATA_DBL:
      PROTECT(tmp=allocVector(REALSXP,type.size));
      for(i=0;i<type.size;i++) {
	REAL(tmp)[i]=DC_DBL(dc,id)[i];
      }
      break;
    case DATA_INT:
      PROTECT(tmp=allocVector(INTSXP,type.size));
      for(i=0;i<type.size;i++) {
	INTEGER(tmp)[i]=DC_INT(dc,id)[i];
      }
      break;
  }
  defineVar(install(type.key),tmp,renv->env);
  UNPROTECT(1);
}


void cqlsREnv_getVar(CQLS_RENV renv,DATA_CENTER dc,int id) {
  ST_DATA_STRUCT type=dc->type[id];
  SEXP tmp;
  int i;

  switch(type.type) {
    case DATA_DBL:
      //PROTECT(tmp=allocVector(REALSXP,type.size));
      PROTECT(tmp=coerceVector(findVar(install(type.key),renv->env),REALSXP));
      for(i=0;i<type.size;i++) {
	DC_DBL(dc,id)[i]=(DOUBLE)(REAL(tmp)[i]);
      }
      break;
    case DATA_INT:
      //PROTECT(tmp=allocVector(INTSXP,type.size));
      PROTECT(tmp=coerceVector(findVar(install(type.key),renv->env),INTSXP));
      for(i=0;i<type.size;i++) {
	DC_INT(dc,id)[i]=INTEGER(tmp)[i];
      }
      break;
  }
  UNPROTECT(1);
}

void cqlsREnv_setVar_key(CQLS_RENV renv,DATA_CENTER dc,char *key) {
  cqlsR_setVar(renv,dc,cqlsdataCenter_idData(dc,key));
}

void cqlsREnv_getVar_key(CQLS_RENV renv,DATA_CENTER dc,char *key) {
  cqlsR_getVar(renv,dc,cqlsdataCenter_idData(dc,key));
}


void cqlsREnv_setAllVars_List(CQLS_RENV renv,char *listName,DATA_CENTER dc) {
  ST_DATA_STRUCT type;
  SEXP tmp,name,res;
  int i,id,created=0;

  res=findVar(install(listName),renv->env);
  if(res==R_UnboundValue) {
    //Rprintf("%s created!!!\n",listName);
    PROTECT(res=allocVector(VECSXP,dc->n));
    PROTECT(name=allocVector(STRSXP,dc->n));
    created=1;
  }
  for(id=0;id<dc->n;id++) {
    type=dc->type[id];
    switch(type.type) {
      case DATA_DBL:
	PROTECT(tmp=allocVector(REALSXP,type.size));
	for(i=0;i<type.size;i++) {
	  REAL(tmp)[i]=DC_DBL(dc,id)[i];
	}
	break;
      case DATA_INT:
	PROTECT(tmp=allocVector(INTSXP,type.size));
	for(i=0;i<type.size;i++) {
	  INTEGER(tmp)[i]=DC_INT(dc,id)[i];
	}
	break;
    }
    SET_VECTOR_ELT(res,id,tmp);
    if(created) SET_STRING_ELT(name,id,mkChar(type.key));
    UNPROTECT(1);
  }
  if(created) {
    setAttrib(res,R_NamesSymbol,name);
    defineVar(install(listName),res,renv->env);
    UNPROTECT(2);
  }
}


void cqlsREnv_setAllVars_List_new(CQLS_RENV renv,char *listName,DATA_CENTER dc) {
  ST_DATA_STRUCT type;
  SEXP tmp,name,res;
  int i,id;

  PROTECT(res=allocVector(VECSXP,dc->n));
  PROTECT(name=allocVector(STRSXP,dc->n));
  for(id=0;id<dc->n;id++) {
    type=dc->type[id];
    switch(type.type) {
      case DATA_DBL:
	PROTECT(tmp=allocVector(REALSXP,type.size));
	for(i=0;i<type.size;i++) {
	  REAL(tmp)[i]=DC_DBL(dc,id)[i];
	}
	break;
      case DATA_INT:
	PROTECT(tmp=allocVector(INTSXP,type.size));
	for(i=0;i<type.size;i++) {
	  INTEGER(tmp)[i]=DC_INT(dc,id)[i];
	}
	break;
    }
    SET_VECTOR_ELT(res,id,tmp);
    SET_STRING_ELT(name,id,mkChar(type.key));
    UNPROTECT(1);
  }
  setAttrib(res,R_NamesSymbol,name);
  defineVar(install(listName),res,renv->env);
  UNPROTECT(2);
}


void cqlsREnv_getAllVars_List(CQLS_RENV renv,char *listName,DATA_CENTER dc) {
  SEXP list;

  list=findVar(install(listName),renv->env);
  if(list==R_UnboundValue) return;
  cqlsSEXP_getAllVars_List(list,dc);
}


void cqlsREnv_getAllVars_List_key(CQLS_RENV renv,char *listName,DATA_CENTER dc) {
  SEXP list;

  list=findVar(install(listName),renv->env);
  if(list==R_UnboundValue) return;
  cqlsSEXP_getAllVars_List_key(list,dc);
}


void cqlsREnv_getAllVars_MDF_key(CQLS_RENV renv,char *mdfName,int i,DATA_CENTER dc) {
  SEXP mdf;
  
  mdf=findVar(install(mdfName),renv->env);
  if(mdf==R_UnboundValue) return;
  cqlsSEXP_getAllVars_MDF_key(mdf,i,dc);
}

//CQLS_R session in local env!
CQLS_R cqlsR_new() {
  CQLS_R self;

  self=(CQLS_R)Calloc(1,ST_CQLS_R);
  self->env=CQLSR->env;
  self->envName=NULL;
  return self;
} 

SEXP cqlsR_eval(CQLS_R self,SEXP cmd) {
  int status;
  SEXP ans;

  PROTECT(cmd);
  PROTECT(ans = R_tryEval(cmd,self->env, &status));
  if(status) {
    //fprintf(stderr, "Caught another error calling sqrt()\n");
    ans=R_NilValue;
    fflush(stderr);
  }
  UNPROTECT(2);
  return ans;
}


//CQLS_R with embedded parsed expr!
CQLS_REXPR cqlsRExpr_new() {
  CQLS_REXPR self;

  self=(CQLS_REXPR)Calloc(1,ST_CQLS_REXPR);
  self->env=CQLSR->env;
  self->envName=NULL;
  return self;
}

int cqlsRExpr_parse(CQLS_REXPR self,char *cmdString) {
  self->cmdString=cmdString;
  //VERY IMPORTANT; need to be not considered by the GC of R!!!
  R_PreserveObject(self->cmd=CQLSR_parse1(self->cmdString));
  if(self->cmd==R_NilValue) return 0;
  return 1;
}


SEXP cqlsRExpr_eval(CQLS_REXPR self) {
  int status;

  PROTECT(self->cmd);
  PROTECT(self->ans = R_tryEval(VECTOR_ELT(self->cmd, 0),self->env, &status));
  if(status) {
    //fprintf(stderr, "Caught another error calling sqrt()\n");
    self->ans=R_NilValue;
    fflush(stderr);
  }
  UNPROTECT(2);
  return self->ans;
}
