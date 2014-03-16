#ifndef _cqlsvector_h
#define _cqlsvector_h
#include "cqlsR.h"
#include "eblist.h"

#define SHOW_VECTOR(vect,len,title,i) \
printf("%s=(",title);for(i=0;i<len;i++) printf("%LF%s",vect[i],(i==len-1 ? ")\n" : ","));


////////////////////////////////////////////////////// GENERAL
//class Struct Vector

typedef struct st_cqlsVector {
  int nbVector;
  int nbBlockVector;
  int* sizeBlockVector;
  char** keyBlockVector;
  DOUBLE* ansCache; 
  SEXP* getteR;
  SEXP* setteR;
  CQLS_R envR;
} ST_CQLS_VECTOR,*CQLS_VECTOR;

extern CQLS_VECTOR cqlsVector_new();
extern void cqlsVector_free();
extern int cqlsVector_realloc();
extern void cqlsVector_add_block();
extern void cqlsVector_append();
extern void cqlsVector_eval(); //only eval (on the R side)
extern DOUBLE* cqlsVector_newVector();
extern DOUBLE*  cqlsVector_copyVector();
extern void cqlsVector_initVector();
extern void cqlsVector_initVector_fromCache();
extern short cqlsVector_same_vectors();
extern void cqlsVector_vector_increased_by_weighted_vector();
extern void cqlsVector_from_envR_to_vector_after_update();//eval (on the R side), get (on the C side)
extern void cqlsVector_from_envR_to_vector();
extern DOUBLE* cqlsVector_from_envR_to_new_vector_after_update();//eval (on the R side), get (on the C side)
extern void cqlsVector_vector_increased_by_weighted_updated_vector_from_envR();
extern DOUBLE* cqlsVector_from_envR_to_new_vector();//get (on the C side from the R side)
extern void cqlsVector_from_vector_to_envR();//put C vector (on the R side)
extern SEXP cqlsVector_vector_to_R_as_named_list();
extern void cqlsVector_initCache();
extern void cqlsVector_from_envR_to_vector_after_update();//eval (on the R side), get (on the C side)
extern void cqlsVector_from_envR_to_cache();
extern void cqlsVector_from_cache_to_envR();//put the C cache (on the R side)
extern SEXP cqlsVector_cache_to_R_as_named_list();
extern void cqlsVector_cache_increased_by_weighted_updated_vector_from_envR();
extern void cqlsVector_show();
extern void cqlsVector_print();
extern void cqlsVector_print_cache();
extern SEXP cqlsVector_vector_array_to_R_as_dataframe();

//class Struct VectorList
typedef struct st_cqlsVectorList {
  CQLS_VECTOR vector;
  PT_LIST list;
} ST_CQLS_VECTOR_LIST,*CQLS_VECTOR_LIST;

extern CQLS_VECTOR_LIST cqlsVectorList_new();
extern void cqlsVectorList_free();
extern void cqlsVectorList_insert();
extern void cqlsVectorList_empty();
extern SEXP cqlsVectorList_to_R_as_dataframe();
extern void cqlsVectorList_print();
#endif
