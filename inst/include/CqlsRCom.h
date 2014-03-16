#ifndef _cqlsRCom_h
#define _cqlsRCom_h
#include <Rinternals.h>
#include "CqlsRCom.h"
//from CqlsRCom.c
/**** The environment rho or obj are already coerced vectors!!! ****/
extern SEXP C_getVarInEnvir(char* name, SEXP rho);
extern SEXP C_setVarInEnvir(char* name, SEXP value, SEXP rho);
extern SEXP C_defVarInEnvir(char* name, SEXP value, SEXP rho);
#define C_getCqlsObjVar(obj,name) C_getVarInEnvir(name,obj)
#define C_setCqlsObjVar(obj,name,value) C_setVarInEnvir(name,value,obj)
#define C_defCqlsObjVar(obj,name,value) C_defVarInEnvir(name,value,obj)
#define C_getCqlsObjExternalPtr(obj) C_getVarInEnvir("extPtr",obj)

/**** obj is a environment to be coerced ****/
extern SEXP C_getVarForCqlsObj(SEXP obj,char* name);
extern SEXP C_defVarForCqlsObj(SEXP obj,char* name, SEXP value);
#define C_getExternalPtrForCqlsObj(obj) C_getVarForCqlsObj(obj,"extPtr")

/**** externalPtr has an attribute envir to be coerced ****/
extern SEXP C_getVarForExternalPtr(SEXP externalPtr, char* name); //attr(externalPtr,"envir")$name
extern SEXP C_setVarForExternalPtr(SEXP externalPtr, char* name, SEXP value);
extern SEXP C_defVarForExternalPtr(SEXP externalPtr, char* name, SEXP value);
#define C_getExternalPtrForExternalPtr(externalPtr,name) C_getExternalPtrForCqlsObj(C_getVarForExternalPtr(externalPtr,name))


#endif
