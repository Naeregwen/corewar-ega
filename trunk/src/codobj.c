/****************************
 * Projet  : corewar        *
 * Fichier : codobj.c       *
 * Traitement du code objet *
 * Auteur  : NVA            *
 * Date    : 20/11/94       *
 ****************************/

#include <stdlib.h>

#include "inc/codobj.h"
/*#include "redcodop.h"*/

/****************************************
 * Creation d'un nouvel element         *
 *                                      *
 * entree : un code operation           *
 *                                      *
 * sortie : un lien sur un element      *
 ****************************************/

codobj *codobjcreer (char codop) {

register codobj *nouv=(codobj*) calloc (1, sizeof (codobj));

	if ( !nouv )
		return NULL; /* plus de place en memoire */

	nouv->instr.codop = codop;
	nouv->argires[0] = nouv->argires[1] = NULL;
	nouv->suiv = NULL;
	return nouv;
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

codobj *codobjchainer (codobj *elem, codobj *liste) {

register codobj *tmpcod=liste;

	if (!elem) return liste;

	/* se placer sur le dernier element */
	while ( tmpcod && tmpcod->suiv) tmpcod = tmpcod->suiv;

	/* chainage */
	if (tmpcod)
		tmpcod->suiv = elem;
	else
		return elem;

	return liste;
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

codobj *codobjtete (codobj *elem, codobj *liste) {

	if (!elem) return liste;
	elem->suiv  = liste;
	return elem;
}

/***********************************
 * Suppression d'un element        *
 *                                 *
 * entree : un lien sur un element *
 *                                 *
 * sortie : un lien sur le suivant *
 *          ou NULL si !elem|!suiv *
 ***********************************/

codobj *codobjeffacer (codobj *elem) {

register codobj *suiv=NULL;

	if (!elem) return NULL;

	suiv = elem->suiv;
	/* les elements de argires ne sont pas les vrais liens sur les chaines,
		on ne les detruit pas */
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

codobj *codobjdetruire (codobj *liste) {

	while ( liste )
		liste = codobjeffacer(liste);
	return liste;
}
