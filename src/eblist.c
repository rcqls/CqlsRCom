/*******************************************/
/*****               LIST             *****/                         /*******************************************/



/**************************************************/
/*              INCLUDE INSERT FILES              */
/**************************************************/
#include <stdio.h>
//#include <malloc.h>
#include <stdlib.h>

#include "eblist.h" 
#include <R.h>

/**************************************************/
/*                      INS_TETE                  */
/**************************************************/
void eblist_ins_tete(PT_LIST *f,void *pt)
{ 
    PT_LIST pt_cel;
    pt_cel = (PT_LIST)Calloc(1,LIST);
    if(pt_cel == NULL)
    {
	printf("ERREUR : PLUS DE PLACE POUR LES POINTS");
	exit(1);
    }
    pt_cel->pt_cur = pt;
    pt_cel->pt_next = *f;
    *f = pt_cel;
}  

/**************************************************/
/*                    INS_ORDONNEE                */
/**************************************************/
void  eblist_ins_ordonnee(PT_LIST *f,void *pt_v)
{
    PT_LIST pt_cel,pt_prec,pt;

    if(*f == NULL)
    {
	pt_cel = (PT_LIST)Calloc(1,LIST);
	if(pt_cel == NULL)
	{
	    Rprintf("ERREUR : PLUS DE PLACE POUR LES POINTS");
	    exit(1);
	}
	pt_cel->pt_cur = pt_v;
	pt_cel->pt_next = *f;
	*f = pt_cel;
	return;
    }
    if((*f)->pt_cur > pt_v)
    {
	pt_cel = (PT_LIST)Calloc(1,LIST);
	if(pt_cel == NULL)
        {
	    Rprintf("ERREUR : PLUS DE PLACE POUR LES POINTS");
	    exit(1);
        }
	pt_cel->pt_cur = pt_v;
	pt_cel->pt_next = *f;
	*f = pt_cel;
	return;
    }
    //element non unique!!!
    if((*f)->pt_cur == pt_v) return;
    for(pt_prec = *f,pt = (*f)->pt_next;pt != NULL;pt = pt->pt_next,pt_prec = pt_prec->pt_next) {
      if(pt->pt_cur >= pt_v) break;
    }
    //element non unique si à la sortie du for il y a un break avec égalité!!!
    if(pt!=NULL && pt->pt_cur == pt_v) return;
    pt_cel = (PT_LIST)Calloc(1,LIST);
    if(pt_cel == NULL)
    {
	printf("ERREUR : PLUS DE PLACE POUR LES POINTS");
	exit(1);
    }
    pt_cel->pt_cur = pt_v;
    pt_cel->pt_next = pt;
    pt_prec->pt_next = pt_cel;   
}




/**************************************************/
/*                    INS_SORTED                  */
/**************************************************/
//extra is called in cmp function!
/*int  eblist_ins_sorted(PT_LIST *f,void *pt_v,int (*cmp)(void*,void*,void*),void *extra)
{
  PT_LIST pt_cel,pt_prec,pt;

  if(*f == NULL)
  {
    pt_cel = (PT_LIST)Calloc(1,LIST);
    if(pt_cel == NULL)
    {
      Rprintf("ERREUR : PLUS DE PLACE POUR LES POINTS");
      exit(1);
    }
    pt_cel->pt_cur = pt_v;
    pt_cel->pt_next = *f;
    *f = pt_cel;
    return 1;
  }
  if(cmp((*f)->pt_cur,pt_v,extra)>0)
  {
    pt_cel = (PT_LIST)Calloc(1,LIST);
    if(pt_cel == NULL)
    {
      Rprintf("ERREUR : PLUS DE PLACE POUR LES POINTS");
      exit(1);
    }
    pt_cel->pt_cur = pt_v;
    pt_cel->pt_next = *f;
    *f = pt_cel;
    return -1;
  }
    //element non unique!!!
  if(cmp((*f)->pt_cur, pt_v,extra)==0) return 0;
  for(pt_prec = *f,pt = (*f)->pt_next;pt != NULL;pt = pt->pt_next,pt_prec = pt_prec->pt_next) {
    if(cmp(pt->pt_cur, pt_v,extra)>=0) break;
  }
    //element non unique si à la sortie du for il y a un break avec égalité!!!
  if(pt!=NULL && (cmp(pt->pt_cur,pt_v,extra)==0)) return 0;
  pt_cel = (PT_LIST)Calloc(1,LIST);
  if(pt_cel == NULL)
  {
    Rprintf("ERREUR : PLUS DE PLACE POUR LES POINTS");
    exit(1);
  }
  pt_cel->pt_cur = pt_v;
  pt_cel->pt_next = pt;
  pt_prec->pt_next = pt_cel;
  return 1;
}*/

/**************************************************/
/*                INS_SI_NOT_IN                   */
/**************************************************/
void  eblist_ins_si_not_in(PT_LIST *f,void *pt)
{ 
PT_LIST pt_cellule; 
if(eblist_dans(pt,*f))
    return;
pt_cellule = (PT_LIST)Calloc(1,LIST);

if(pt_cellule == NULL)
    {
    Rprintf("ERREUR : PLUS DE PLACE POUR LES POINTS");
    exit(1);
    }
pt_cellule->pt_cur = pt;
pt_cellule->pt_next = *f;
*f = pt_cellule;
}


/**************************************************/
/*                  SUPPRIME_DE_LIST             */
/**************************************************/
void  eblist_supprime(PT_LIST *list,void *pt_sup)
{
  PT_LIST pt,pt_prec,pt_temps;
  
  if (*list == NULL) 
    return;
  if (pt_sup == (*list)->pt_cur)
    {
      pt_temps = *list;
      *list = (*list)->pt_next;
      Free(pt_temps);
      return;
    }
  for(pt_prec = *list,pt = (*list)->pt_next; pt!=NULL; pt = pt->pt_next,pt_prec = pt_prec->pt_next)
    {
      if(pt->pt_cur == pt_sup)
	{
	  pt_prec->pt_next = pt->pt_next; 
	  Free(pt);
	  return;  
	}
    }
}

/**************************************************/
/*                    DUPLIQUER                   */
/**************************************************/
void  eblist_dupliquer(PT_LIST list_a_dupliquer,PT_LIST *list_duplique)
{
    PT_LIST pt;

    for(pt=list_a_dupliquer;pt!=NULL;pt=pt->pt_next)
    {    
        eblist_ins_ordonnee(list_duplique,(void*)(pt->pt_cur));
    }
}

/****************************************/
/* 		INS_QUEUE						*/
/****************************************/
void  eblist_ins_queue(PT_LIST *f,void *pt)
{
PT_LIST pt_cellule,p = *f,prec;
pt_cellule = (PT_LIST)Calloc(1,LIST);
pt_cellule->pt_next = NULL;
if(pt_cellule == NULL)
	{
	printf("ERREUR PLUS DE PLACE POUR LA SUITE DES POINTS");
	getchar();
	exit(0);
	}

pt_cellule->pt_cur = pt;

if(*f == NULL)
	/*	INSERTION EN TETE	*/
	{
	pt_cellule->pt_next = * f;
	*f = pt_cellule;
	}
else
	{ /* en attendant mieu	*/
	while(p != NULL)
		{
		prec = p;
		p = p->pt_next;
		}
	prec->pt_next = pt_cellule;
	}
}



/**************************************************/
/*                       VIDER                    */
/**************************************************/
void  eblist_vider(PT_LIST *list) {
    PT_LIST temps;
    while(*list != NULL) {
	    temps = *list;
	    *list = (*list)->pt_next;
	    Free(temps); //ATTENTION: Seul le noeud est libéré pas son contenu! => utiliser recup_tete!!!
    }
}


/**************************************************/
/*                    RECUP_TETE                  */
/**************************************************/
void*  eblist_recup_tete(PT_LIST *list) {
  PT_LIST temps;
  void * pt;
  //printf("ici4\n");
  if (*list == NULL)
    {
      Rprintf("\nerreur rien a recuperer\n");
      return(NULL);
    }
  
  temps = *list;           
  //printf("ici4\n");  
  *list = (*list)->pt_next;
  //printf("ici4\n");
  pt=temps->pt_cur;
  //printf("ici4\n");
  Free(temps); 
  //printf("ici4\n");
  return(pt);
}  

/**************************************************/
/*                  DANS_LA_LIST                 */
/**************************************************/
int  eblist_dans(void *pt,PT_LIST list) 
{  
    PT_LIST pt_list;
    for(pt_list=list;pt_list!=NULL;pt_list=pt_list->pt_next) {
	    if(pt_list->pt_cur == pt) return(1);
    } 
    return(0);
} 


/**************************************************/
/*          TROUVER DANS_LA_LIST                 */
/**************************************************/
int eblist_find(PT_LIST list,void *pt) { 
    PT_LIST pt_list;
    int i;
    
    for(pt_list=list,i=0;pt_list!=NULL;pt_list=pt_list->pt_next,i++) { 
      if(pt_list->pt_cur == pt) return(i);
    } 
    return(-1);
} 

/**************************************************/
/*          TROUVER DANS_LA_LIST                 */
/**************************************************/
void*  eblist_extract(PT_LIST list,int pos) { 
    PT_LIST pt_list;
    int i;
    
    for(pt_list=list,i=0;pt_list!=NULL;pt_list=pt_list->pt_next,i++);
    return( (pt_list==NULL ? NULL : pt_list->pt_cur) );
} 



/*******************************************************/
/*          Combien supérieur                                               */
/*******************************************************/
int  eblist_combien_superieur_a(void *pt,PT_LIST list) {
  PT_LIST pt_list;
  int j=0;  
    
  for(pt_list=list;pt_list!=NULL;pt_list=pt_list->pt_next)
  {
    if(pt_list->pt_cur > pt) break;
    j++;
  } 
  return(j);
}

/**************************************************/
/*                  LONGUEUR                */
/**************************************************/
int  eblist_length(PT_LIST list)
{ 
  PT_LIST pt_list;
  int cpt=0;  
  //printf("cpt=%d\n",cpt);
  for(pt_list=list;pt_list!=NULL;pt_list=pt_list->pt_next)
  {
    cpt++;
    //printf("cpt=%d\n",cpt);
  } 
  return(cpt);
} 
