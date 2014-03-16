#ifndef _eblist_h
#define _eblist_h

/* Structure des lists */
typedef struct st_list {
  void   *pt_cur;
  struct st_list  *pt_next;
}LIST,*PT_LIST;


extern void eblist_ins_tete(); 
extern void eblist_ins_ordonnee();
//extern void eblist_ins_sorted();
extern void eblist_control_sommet();
extern void eblist_control_point();
extern void eblist_supprime();
extern void eblist_dupliquer();
extern void eblist_vider();
extern void eblist_ins_si_not_in();
extern void eblist_remplacer();
extern void * eblist_recup_tete();
extern int eblist_dans();     
extern int eblist_combien_superieur_a();   
extern int eblist_length();
#endif
