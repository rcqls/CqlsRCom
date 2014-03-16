#ifndef _cqlsR_h
#define _cqlsR_h
#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>
#include <R_ext/Parse.h>
#include <Rversion.h>
#include <math.h>

#if (R_VERSION < 132352) /* before 2.5 to check! */
//SEXP R_ParseVector(SEXP, int, int *);
#define RR_ParseVector(x,y,z) R_ParseVector(x, y, z)
#else
//SEXP R_ParseVector(SEXP, int, int *,SEXP);
#define RR_ParseVector(x,y,z) R_ParseVector(x, y, z, R_NilValue)
#endif

SEXP NewEnvironment(SEXP,SEXP,SEXP);

#define DOUBLE long double
#define LF Lf
#define EXP expl

//#define infini (DOUBLE)10000000000000.;
#define infini INFINITY

typedef struct st_data {
  void **data;///external data
}ST_DATA,*DATA;
extern void* cqlsdata_get(); //args=DATA,ID
extern DOUBLE* cqlsdata_DBL();
extern int* cqlsdata_INT();

//type of data
typedef enum {
  DATA_DBL,
  DATA_INT,
  DATA_NULL
} DATA_TYPE;

//DataStruct: component used to describe
typedef struct st_dataStruct{
  int id;//corresponds to the index in the variable type of dataCenter!!! 
  char* key;
  int   size;
  DATA_TYPE type;
}ST_DATA_STRUCT,*DATA_STRUCT;

//DataCenter to declare external data and store it (if data is not NULL)
typedef struct st_dataCenter{
  void **data;//if not NULL the data are stored here!!!
  int n;
  DATA_STRUCT type;//dynamic array of data structure!
  DATA curObj;//the current obj with data field
}ST_DATA_CENTER,*DATA_CENTER;
extern DATA_CENTER cqlsdataCenter_new();
extern void cqlsdataCenter_free();
extern int cqlsdataCenter_declare();//args=nb of data to declare;return the first id
extern void cqlsdataCenter_newData();
extern void cqlsdataCenter_freeData();
extern void cqlsdataCenter_show();
extern int cqlsdataCenter_curObj();//used in ebvor!!!!
extern void cqlsdataCenter_newDataForObj();
extern void cqlsdataCenter_freeDataForObj();
extern DOUBLE* cqlsdataCenter_DBL();
extern int* cqlsdataCenter_INT();
extern int cqlsdataCenter_size();
extern int cqlsdataCenter_idData();
extern DOUBLE* cqlsdataCenter_DBL_key();
extern int* cqlsdataCenter_INT_key();
extern int cqlsdataCenter_size_key();
#define DC_declare(dc,size,type,key) cqlsdataCenter_declare(dc,size,type,key)
#define DC_newData(dc) cqlsdataCenter_newData(dc)
#define DC_freeData(dc) cqlsdataCenter_freeData(dc)
#define DC_curObj(dc,obj) cqlsdataCenter_curObj(dc,obj)
#define DC_DBL(dc,id) cqlsdataCenter_DBL(dc,id)
#define DC_INT(dc,id) cqlsdataCenter_INT(dc,id)
#define DC_size(dc,id) cqlsdataCenter_size(dc,id)
#define DC_type(dc,id) dc->type[id]
#define DC_idData(dc,key) cqlsdataCenter_idData(dc,key)
#define DC_DBL_key(dc,key) cqlsdataCenter_DBL_key(dc,key)
#define DC_INT_key(dc,key) cqlsdataCenter_INT_key(dc,key)
#define DC_size_key(dc,key) cqlsdataCenter_size_key(dc,key)

//The goal is that several DataUser may use the same DataCenter
//However the ids of the DataCenter may differ from the ids of the DataUser 
//The ids of  are local to each DataUser
//DataUser to use the external data
typedef struct st_dataUser{
  DATA_CENTER dc;
  int nbData; 
  int *idData;//idData[idUser] is the id of the data in dc!
}ST_DATA_USER,*DATA_USER;
extern DATA_USER cqlsdataUser_new();
extern void cqlsdataUser_free();
extern int cqlsdataUser_declare();
extern DOUBLE* cqlsdataUser_DBL();
extern int* cqlsdataUser_INT();
extern int cqlsdataUser_idData();
extern DOUBLE* cqlsdataUser_DBL_key();
extern int* cqlsdataUser_INT_key();
#define DU_declare(du,size,type,key) cqlsdataUser_declare(du,size,type,key)
#define DU_DBL(du,id) cqlsdataUser_DBL(du,id)
#define DU_INT(du,id) cqlsdataUser_INT(du,id)
#define DU_type(du,id) du->dc->type[du->idData[id]]
#define DU_idData(du,key) cqlsdataUser_idData(du,key) 

//SEXP util
extern DOUBLE cqlsSEXP_DBL();
extern int cqlsSEXP_INT();
extern void cqlsSEXP_setAllVars_List();
extern void cqlsSEXP_setAllVarsNA_List();
extern void cqlsSEXP_getAllVars_List();
extern void cqlsSEXP_setAllVars_List_key();
extern void cqlsSEXP_getAllVars_List_key();
extern void cqlsSEXP_setAllVars_MDF();
extern void cqlsSEXP_setAllVarsNA_MDF();
extern void cqlsSEXP_getAllVars_MDF();
extern void cqlsSEXP_setAllVars_MDF_key();
extern void cqlsSEXP_getAllVars_MDF_key();
extern void DU_cqlsSEXP_setAllVars_List();
extern void DU_cqlsSEXP_setAllVarsNA_List();
extern void DU_cqlsSEXP_getAllVars_List();
extern void DU_cqlsSEXP_setAllVars_List_key();
extern void DU_cqlsSEXP_getAllVars_List_key();
extern void DU_cqlsSEXP_setAllVars_MDF();
extern void DU_cqlsSEXP_setAllVarsNA_MDF();
extern void DU_cqlsSEXP_getAllVars_MDF();
extern void DU_cqlsSEXP_setAllVars_MDF_key();
extern void DU_cqlsSEXP_getAllVars_MDF_key();


//cqlsREnv Wrapper!
typedef struct st_cqlsREnv{
  SEXP env;
  char *envName;
} *CQLS_RENV;
extern void cqlsREnv_setEnv();
extern SEXP cqlsREnv_eval1();
SEXP cqlsREnv_DATA2SEXP();
SEXP cqlsREnv_DBL2SEXP();
SEXP cqlsREnv_INT2SEXP();
extern void cqlsREnv_setVar();
extern void cqlsREnv_getVar();
extern void cqlsREnv_setVar_key();
extern void cqlsREnv_getVar_key();
extern void cqlsREnv_setAllVars_List();
extern void cqlsREnv_setAllVars_List_new();
extern void cqlsREnv_getAllVars_List();
extern void cqlsREnv_getAllVars_List_key();
extern void cqlsREnv_getAllVars_MDF();
extern void cqlsREnv_getAllVars_MDF_key();

//cqlsRSession: unique global R Session
typedef struct st_cqlsRSession{
  SEXP env;
  //char **argv;
  //int argc;
}ST_CQLS_RSESSION,*CQLS_RSESSION;
//Global variables
static CQLS_RSESSION CQLSR=NULL;
//static int CQLSR_WITHIN_R=0;
extern void CQLSR_start();
//extern void CQLSR_within_R();
extern int CQLSR_init();
//extern void CQLSR_args();
extern SEXP CQLSR_parse1();//one line
extern SEXP CQLSR_DATA2SEXP();
extern SEXP CQLSR_DBL2SEXP();
extern SEXP CQLSR_INT2SEXP();
extern void CQLSR_setVar();
extern void CQLSR_getVar();
extern void CQLSR_setVar_key();
extern void CQLSR_getVar_key();
extern void CQLSR_setAllVars_List();
extern void CQLSR_getAllVars_List();
extern void CQLSR_getAllVars_List_key();
extern void CQLSR_getAllVars_MDF();
extern void CQLSR_getAllVars_MDF_key();
extern SEXP CQLSR_eval1();

//NO DataUser, only one DataCenter
typedef struct st_cqlsR{
  SEXP env;
  char* envName;
  //datacenter later!!!
}ST_CQLS_R,*CQLS_R;
extern CQLS_R cqlsR_new();
extern SEXP cqlsR_eval();
#define cqlsR_DATA2SEXP cqlsREnv_DATA2SEXP
#define cqlsR_DBL2SEXP cqlsREnv_DBL2SEXP
#define cqlsR_INT2SEXP cqlsREnv_INT2SEXP
#define cqlsR_setVar cqlsREnv_setVar
#define cqlsR_getVar cqlsREnv_getVar
#define cqlsR_setVar_key cqlsREnv_setVar_key
#define cqlsR_getVar_key cqlsREnv_getVar_key
#define cqlsR_setAllVars_List cqlsREnv_setAllVars_List
#define cqlsR_setAllVars_List_new cqlsREnv_setAllVars_List_new
//#define cqlsR_setAllVars_Lists cqlsREnv_setAllVars_Lists
#define cqlsR_getAllVars_List cqlsREnv_getAllVars_List
#define cqlsR_getAllVars_List_key cqlsREnv_getAllVars_List_key
#define cqlsR_getAllVars_MDF cqlsREnv_getAllVars_MDF
#define cqlsR_getAllVars_MDF_key cqlsREnv_getAllVars_MDF_key
#define cqlsR_eval1(cqlsr,cmdString) cqlsREnv_eval1((CQLS_RENV)cqlsr,cmdString)
#define cqlsR_setEnv cqlsREnv_setEnv

typedef struct st_cqlsRExpr{
  SEXP env;
  SEXP envName;
  char *cmdString;
  SEXP ans;//You need to cast the answer!!!
  SEXP cmd;
}ST_CQLS_REXPR, *CQLS_REXPR;
extern CQLS_REXPR cqlsRExpr_new();
extern int cqlsRExpr_parse();
extern SEXP cqlsRExpr_eval();
#define cqlsRExpr_setEnv cqlsREnv_setEnv
#define cqlsRExpr_DATA2SEXP cqlsREnv_DATA2SEXP
#define cqlsRExpr_DBL2SEXP cqlsREnv_DBL2SEXP
#define cqlsRExpr_INT2SEXP cqlsREnv_INT2SEXP
#define cqlsRExpr_setVar cqlsREnv_setVar
#define cqlsRExpr_getVar cqlsREnv_getVar
#define cqlsRExpr_setVar_key cqlsREnv_setVar_key
#define cqlsRExpr_getVar_key cqlsREnv_getVar_key
#define cqlsRExpr_setAllVars_List cqlsREnv_setAllVars_List
#define cqlsRExpr_getAllVars_List cqlsREnv_getAllVars_List
#define cqlsRExpr_getAllVars_List_key cqlsREnv_getAllVars_List_key
#define cqlsRExpr_getAllVars_MDF cqlsREnv_getAllVars_MDF
#define cqlsRExpr_getAllVars_MDF_key cqlsREnv_getAllVars_MDF_key

//reactivable external pointer for R
extern void* cqlsR_ExternalPtrAddr();
#endif
