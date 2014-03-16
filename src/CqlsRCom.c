#include "CqlsRCom.h"

#include <R_ext/Rdynload.h>

//get from the R doc! (see the R doc for further explanation)
SEXP C_getVarInEnvir(char* name, SEXP rho) {
  SEXP ans;

  //if(!isString(name) || length(name) != 1) error("name is not a single string");
  if(!isEnvironment(rho)) error("rho should be an environment");
  ans = findVar(install(name), rho);
  //Rprintf("first value is %f\n", REAL(ans)[0]);
  return(ans);
}

SEXP C_setVarInEnvir(char* name, SEXP value, SEXP rho) {
  //if(!isString(name) || length(name) != 1) error("name is not a single string");
  if(!isEnvironment(rho)) error("rho should be an environment");
  setVar(install(name),value,rho);
  return R_NilValue;
}

SEXP C_defVarInEnvir(char* name, SEXP value, SEXP rho) {
  //if(!isString(name) || length(name) != 1) error("name is not a single string");
  if(!isEnvironment(rho)) error("rho should be an environment");
  defineVar(install(name),value,rho);
  return R_NilValue;
}

SEXP C_getVarForExternalPtr(SEXP externalPtr, char* name) {
  SEXP rhoR,ansR;

  PROTECT(rhoR=coerceVector(getAttrib(externalPtr,install("envir")),ENVSXP));
  ansR=C_getVarInEnvir(name,rhoR);
  UNPROTECT(1);
  return ansR;
}

SEXP C_setVarForExternalPtr(SEXP externalPtr, char* name, SEXP value) {
  SEXP rhoR;

  PROTECT(rhoR=coerceVector(getAttrib(externalPtr,install("envir")),ENVSXP));
  C_setVarInEnvir(name,value,rhoR);
  UNPROTECT(1);
  return R_NilValue;  
}

SEXP C_defVarForExternalPtr(SEXP externalPtr, char* name, SEXP value) {
  SEXP rhoR;

  PROTECT(rhoR=coerceVector(getAttrib(externalPtr,install("envir")),ENVSXP));
  C_defVarInEnvir(name,value,rhoR);
  UNPROTECT(1);
  return R_NilValue;  
}


SEXP C_getVarForCqlsObj(SEXP obj,char* name) {
  SEXP rhoR,ansR;

  PROTECT(rhoR=coerceVector(obj,ENVSXP));
  ansR=C_getVarInEnvir(name,rhoR);
  UNPROTECT(1);
  return ansR;
}

SEXP C_defVarForCqlsObj(SEXP obj,char* name, SEXP value) {
  SEXP rhoR;

  PROTECT(rhoR=coerceVector(obj,ENVSXP));
  C_defVarInEnvir(name,value,rhoR);
  UNPROTECT(1);
  return R_NilValue;
}

//to test the result of 
SEXP R_getVarForCqlsObj(SEXP args) {
  SEXP rhoR,ansR,nameR;

  PROTECT(rhoR=coerceVector(CADR(args),ENVSXP));
  PROTECT(nameR=coerceVector(CADDR(args),STRSXP));
  ansR=C_getVarInEnvir(CHAR(STRING_ELT(nameR,0)),rhoR);
  UNPROTECT(2);
  return ansR;
}

static const R_ExternalMethodDef externalMethods[] = {
  {"getVarForCqlsObj",(DL_FUNC) &R_getVarForCqlsObj,-1},
  {NULL,NULL,0}
};


void R_init_CqlsRCom(DllInfo *dll) {
  R_registerRoutines(dll,NULL,NULL,NULL,externalMethods); //callMethods in 2nd position!
  R_useDynamicSymbols(dll, FALSE);
}
