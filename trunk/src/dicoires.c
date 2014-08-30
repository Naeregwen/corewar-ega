/******************************************
 * Projet  : corewar                      *
 * Fichier : dicoires.c                   *
 * Dictionnaire d'etiquettes non resolues *
 * Auteur  : NVA                          *
 * Date    : 24/11/94                     *
 ******************************************/

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
#define DICOIRESMAX 100

/* elements du dictionnaire */
/* etiquette n'ayant pu etre trouve dans le dicoetiq */
/* lors de la premiere lecture */

typedef struct tires {

	char *nom; /* reference */
	struct tires *suiv;
	struct tires *prec;

} ires;


/* dictionnaire */
ires **dicoires;

/*************/
/* fonctions */
/*************/

/* fonction de hachage reellement utilisee */
#define HIRES(nom) htripl(nom, DICOIRESMAX)

/**************************/
/* initialisation du dico */
/**************************/

short dicoirescreer(void) {

register int i=0; /* indice de case du tableau */

	/* creer le dictionnaire */
	if ( (dicoires = (ires**) malloc (sizeof(ires *) * DICOIRESMAX)) == NULL )
		return 0;

	/* vider les cases */
	for ( i=0; i<DICOIRESMAX; i++ )
		dicoires[i] = NULL;

	return 1;
}

/**********************/
/* effacement du dico */
/**********************/

void dicoireseffacer(void) {

register int i=0; /* indice de case du tableau */
register ires *nres=NULL; /* pointeur d'etiquette */

	for ( i=0; i<DICOIRESMAX; i++ )
		for (nres=dicoires[i]; dicoires[i]; nres=dicoires[i]) {

			dicoires[i]=nres->suiv; /* la case du dictionnaire concerve le lien */
			free(nres->nom);
			free(nres);
		}
}


/***********************/
/* destruction du dico */
/***********************/

void dicoiresdetruire(void) {

	dicoireseffacer();
	free(*dicoires);
}

/*****************************/
/* recherche d'une etiquette */
/*****************************/

ires *iresexiste(char *nom) {

register ires *e=NULL; /* pointeur d'etiquette du dico */

	for ( e=dicoires[HIRES(nom)]; e; e=e->suiv )
		if (strcmp(nom,e->nom) == 0) return e;
	return NULL;

}

/****************************/
/* creation d'une etiquette */
/****************************/

ires *irescreer(char *nom) {

register ires *n=(ires *) calloc(1,sizeof(ires)); /* pointeur de la nouvelle etiquette */

	if (!n || !(n->nom = (char *) calloc(1, strlen(nom)+1))
			|| !(strcpy(n->nom, nom)) )
		return NULL;

	n->suiv = n->prec = NULL;

	return n;
}

/*****************************/
/* insertion d'une etiquette */
/*****************************/

char *ireslier(char *nom) {

register ires *e=iresexiste(nom); /* pointeur d'etiquette du dico */
register int i=HIRES(nom); /* indice de hachage */

	/* chercher si elle existe deja */
	if ( (e = iresexiste(nom)) != NULL )
		return e->nom;

	/* creer la cellule et le premier element de sa liste */
	if ( (e = irescreer(nom)) == NULL )
		return NULL;

	/* chainer en tete de liste */
	if ( !dicoires[i] )
		dicoires[i] = e;
	else {

		e->suiv = dicoires[i];
		dicoires[i]->prec = e;
		dicoires[i] = e;
	}

	return e->nom; /* adresse directe de la chaine */

}

