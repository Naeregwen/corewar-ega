/*****************************************
 * Projet  : corewar                     *
 * Fichier : dicoetiq.c                  *
 * Dictionnaire d'etiquettes             *
 * Auteur  : NVA                         *
 * Date    : 20/11/94                    *   
 *****************************************/

#include <stdlib.h>
#include <string.h>

#include "inc/hach.h"

/****************/
/* dictionnaire */
/****************/

/*************************/
/* structures et donnees */
/*************************/

/* taille du dictionnaire */
#define DICOETIQMAX 100

/* elements du dictionnaire */
typedef struct t_etiq {

	char *nom; /* nom associe */
	signed numl; /* numero de ligne ou l'etiq est declaree */
	struct t_etiq *suiv;
	struct t_etiq *prec;

} etiq;

/* dictionnaire */
etiq **dicoetiq;

/*************/
/* fonctions */
/*************/

/* fonction de hachage reellement utilisee */
#define HETIQ(nom) (htripl(nom, DICOETIQMAX))

/**************************/
/* initialisation du dico */
/**************************/

short dicoetiqcreer(void) {

register int i=0; /* indice de case */

	/* creer le dictionnaire */
	if ( (dicoetiq = (etiq**) malloc (sizeof(etiq *) * DICOETIQMAX)) == NULL )
		return 0;

	/* vider les cases */
	for (i=0; i<DICOETIQMAX; i++)
		dicoetiq[i]=NULL;
	return 1;
}

/**********************/
/* effacement du dico */
/**********************/

/* pour chaque case du tableau, effacer la liste */

void dicoetiqeffacer(void) {

register int i=0; /* indice de case */
register etiq *e=NULL; /* prochaine etiquette a effacer */

	for (i=0; i<DICOETIQMAX; i++)
		for (e=dicoetiq[i]; e; e=dicoetiq[i]) {

			dicoetiq[i]=e->suiv; /* la case du dictionnaire concerve le lien */
			free(e->nom);
			free(e);
		}
}


/***********************/
/* destruction du dico */
/***********************/

void dicoetiqdetruire(void) {

	dicoetiqeffacer();
	free(*dicoetiq);
}

/**********************/
/* recherche d'un nom */
/**********************/

int etiqexist(char *nom) {

register etiq *e=NULL; /* pointeur de noms du dico */

	for ( e=dicoetiq[HETIQ(nom)]; e; e=e->suiv )
		if (strcmp(nom,e->nom) == 0) return e->numl;
	return 0; /* la ligne 0 n'existe pas */

}

/**********************/
/* recherche d'un nom */
/**********************/

etiq *etiqtrouver(char *nom) {

register etiq *e=NULL; /* pointeur de noms du dico */

	for ( e=dicoetiq[HETIQ(nom)]; e; e=e->suiv )
		if (strcmp(nom,e->nom) == 0) return e;
	return NULL;

}



/**********************/
/* insertion d'un nom */
/**********************/

/* ajout en tete de liste */

short etiqajouter(char *nom, signed numl) {

register etiq *n=NULL; /* pointeur de la nouvelle cellule */
register int i=0; /* indice de hachage du mot */

	if ( etiqexist(nom) ) return 0;
	if ( !(n=(etiq *) calloc(1, sizeof(etiq))) )
		return 0; /* plus de place en memoire */

	/* copie du texte dans la cellule */
	if ( !(n->nom=(char *) calloc(1, strlen(nom)+1)) )
	{
		free(n);
		return 0; /* plus de place en memoire */
	}
	strcpy(n->nom,nom);

	/* copie du numero de ligne ou elle a ete declaree */
	n->numl=numl;

	/* insertion dans la liste */
	i=HETIQ(nom);
	n->prec=NULL;
	n->suiv=dicoetiq[i];
	if (dicoetiq[i]) dicoetiq[i]->prec=n;
	dicoetiq[i]=n;

	return 1; /* un element de plus */

}

/***********************/
/* suppresion d'un nom */
/***********************/

short etiqenlever(char *nom) {

register etiq *e=etiqtrouver(nom); /* pointeur du mot cherche */
register int i=HETIQ(nom); /* valeur de hachage de nom */

	/* suppression */
	if (!e) return 0; /* aucun element a enlever */

	if (e->prec)
		e->prec->suiv=e->suiv;
	else
		/* en tete de liste */
		dicoetiq[i]=e->suiv;

	if (e->suiv) e->suiv->prec=e->prec;

	/* liberation */
	free(e->nom);
	free(e);

	return 1; /* un element de moins */

}


/*********************/
/* nombre d'elements */
/*********************/

long nbetiq(void) {

register int i=0; /* indice du dico */
register etiq *e=NULL; /* pointeur de mots */
register long nb=0; /* resultat */

	for (i=0; i<DICOETIQMAX; i++)
		for (e=dicoetiq[i]; e; e=e->suiv)
			nb++;

	return nb;
}



