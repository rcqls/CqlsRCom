#include <stdio.h>
#include <string.h>
#include "cqlsR.h"

/*
Pour les objets R volatiles qui sont des ExternalPtr!!!
objR est un objet qui doit être réactivable:
-> disposer d'une fonction R_CLASS_reactivate (dans l'interface C<->R) qui appelle le constructeur CLASS_new (dans la librairie C)
-> en attributs, les arguments modifiables dans une session R et ainsi sauvegardés de la même façon qu'un objet R normal!
*/

void* cqlsR_ExternalPtrAddr(SEXP objR, void* (*reactivate)(SEXP)) {
  void *objPtr;

  objPtr=R_ExternalPtrAddr(objR);
  if(objPtr==NULL) {
    //renew object
    objPtr=reactivate(objR);
    //assigne l'adresse extérieure de objR à objPtr!
    R_SetExternalPtrAddr(objR, objPtr);    
  }
  //retourne la nouvelle adresse
  return objPtr;
}

void* cqlsR_ExternalPtrAddr_withCFinalizer(SEXP objR, void* (*reactivate)(SEXP), R_CFinalizer_t C_finalizer) {
  void *objPtr;

  objPtr=R_ExternalPtrAddr(objR);
  if(objPtr==NULL) {
    //renew object
    objPtr=reactivate(objR);
    //assigne l'adresse extérieure de objR à objPtr!
    R_SetExternalPtrAddr(objR, objPtr);
    R_RegisterCFinalizerEx(objR,C_finalizer,TRUE);
  }
  //retourne la nouvelle adresse
  return objPtr;
}
