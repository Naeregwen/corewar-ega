/******************************************
 * Projet  : corewar                      *
 * Fichier : listprog.h                   *
 * liste de points d'entree dans le noyau *
 * Auteur  : NVA                          *
 * Date    : 02/12/94                     *
 ******************************************/

#ifndef _LISTPROG_H_
#define _LISTPROG_H_

typedef struct PC {

	int val; /* valeur du pointeur courant */
	struct PC *suiv;
	struct PC *prec;

} CO;

CO *COcreer(int val);
CO *COeffacer (CO *elem);
CO *COdetruire (CO *liste);
CO *COchainer (CO *elem, CO *liste);
void COechanger(CO **source, CO **dest);

#endif
