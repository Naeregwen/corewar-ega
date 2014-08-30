/******************************************
 * Projet  : corewar                      *
 * Fichier : listprog.c                   *
 * liste de points d'entree dans le noyau *
 * listes circulaires                     *
 ******************************************/

#include <stdlib.h>

#include "inc/listprog.h"

/****************************************
 * Creation d'un nouvel element         *
 *                                      *
 * entree : une valeur de CO            *
 *                                      *
 * sortie : un lien sur un element      *
 ****************************************/

CO *COcreer(int val) {

register CO* nouv = (CO*) calloc (1, sizeof (CO));

	if (!nouv) return NULL;
	nouv->val = val;
	nouv->suiv = nouv->prec = nouv; /* listes circulaires */
	return nouv;
}

/***********************************
 * Suppression d'un element        *
 *                                 *
 * entree : un lien sur un element *
 *                                 *
 * sortie : un lien sur le suivant *
 *          ou NULL si !elem|!suiv *
 ***********************************/

CO *COeffacer (CO *elem) {

register CO *suiv = NULL;

	if (!elem) return NULL;
	if ( elem->suiv != elem ) suiv = elem->suiv;
	elem->suiv->prec = elem->prec;
	elem->prec->suiv = elem->suiv;
	free(elem);
	return suiv;
}

/*************************************
 * Destruction d'une liste d'element *
 *                                   *
 * entree : un pointeur de liste     *
 *                                   *
 * sortie : NULL                     *
 *************************************/

CO *COdetruire (CO *liste) {

	while ( liste )
		liste = COeffacer(liste);
	return liste;
}
 
/*****************************************
 * Chainage d'un nouvel element          *
 * en fin de liste                       *
 *                                       *
 * entree : un lien sur la tete          *
 *                                       *
 * sortie : un lien sur la tete          *
 *          elem si liste vide           *
 *          liste si elem vide ou chaine *
 *****************************************/

CO *COchainer (CO *elem, CO *liste) {

	if (!elem) return liste;
	if (!liste) return elem;

	/* chainage */
	elem->suiv = liste->suiv;
	elem->prec = liste->suiv->prec;
	liste->suiv->prec = elem;
	liste->suiv = elem;

	return liste;
}


void COechanger(CO **source, CO **dest) {

register CO *tete = *source;

	if (!*source) return;

	/* detacher de source */
	tete->suiv->prec = tete->prec;
	tete->prec->suiv = tete->suiv;
	if ( *source != (*source)->suiv )
		*source = (*source)->suiv;
	else
		*source = NULL;

	/* chainer en fin de destination */
	if (*dest) {

		tete->prec = (*dest)->prec;
		tete->suiv = *dest;
		(*dest)->prec->suiv = tete;
		(*dest)->prec = tete;
	}
	else {
		*dest = tete;
		tete->suiv = tete->prec = tete;
	}

}


/*****************************************
 * Chainage d'un nouvel element          *
 * en tete de liste                      *
 *                                       *
 * entree : un lien sur la tete          *
 *                                       *
 * sortie : un lien sur la tete          *
 *          liste si elem vide           *
 *          elem si chaine               *
 *****************************************/

CO *COtete (CO *elem, CO *liste) {

	if (!elem) return liste;
	if (!liste) return elem;

	elem->suiv  = liste;
	elem->prec  = liste->prec;
	liste->prec->suiv = elem;
	liste->prec = elem;

	return elem;
}

