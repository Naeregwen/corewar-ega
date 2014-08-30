/******************************************
 * Projet  : corewar                      *
 * Fichier : core.c                       *
 * Structure d'anneau de memoire          *
 ******************************************/

#include <stdlib.h>
#include <string.h>

/* #include "redcodop.h" type operation */
#define MAXARG 2

#include "inc/core.h" /* taille du tableau */
#include "inc/instruct.h"

/* taille du noyau */
#define TAILLENOYAU (MAXCASE*sizeof(operation))

/* lien sur le bloc d'operations */
operation *noyau = NULL;

/******************************************
 * creer le noyau du combat               *
 *                                        *
 * entree : neant                         *
 *                                        *
 * sortie : 1 noyau cree, 0 impossible    *
 ******************************************/

short noyaucreer(void) {

	if ( (noyau = (operation*) calloc (MAXCASE, sizeof(operation))) == NULL )
		return 0;

	return 1;
}

/******************************************
 * detruire le noyau                      *
 *                                        *
 * entree : neant                         *
 *                                        *
 * sortie : neant                         *
 ******************************************/

void noyaudetruire(void) {

	if (noyau) free(noyau);
}


/******************************************
 * lecture du code operation d'une case   *
 *                                        *
 * entree : numero de case                *
 *                                        *
 * sortie : code operation                *
 ******************************************/

char codeoper (int numcase) {

	return noyau[VALIDE(numcase)].codop;
}


/******************************************
 * lecture du numero du proprietaire de   *
 * la case                                *
 *                                        *
 * entree : numero de case                *
 *                                        *
 * sortie : numero du proprietaire        *
 ******************************************/

char appart (int numcase) {

	return noyau[VALIDE(numcase)].appart;
}

/******************************************
 * lecture du niveau de protection de     *
 * la case                                *
 *                                        *
 * entree : numero de case                *
 *                                        *
 * sortie : niveau de protection          *
 ******************************************/

char protect (int numcase) {

	return noyau[VALIDE(numcase)].protect;
}

/******************************************
 * lecture du mode d'addressage           *
 * d'un argument d'une case               *
 *                                        *
 * entree : numero de case                *
 *                                        *
 * sortie : code operation                *
 ******************************************/

int modeadr (int numcase, int numarg) {

	return noyau[VALIDE(numcase)].tabarg[numarg].adr;
}


/******************************************
 * lecture de la valeur d'un argument     *
 * d'une case                             *
 *                                        *
 * entree : numero de case                *
 *                                        *
 * sortie : code operation                *
 ******************************************/

int valarg (int numcase, int numarg) {

	return noyau[VALIDE(numcase)].tabarg[numarg].val;
}


/* instruction PCT-TST */

void proteger(int numcase, int niv) {

	noyau[VALIDE(numcase)].protect = niv;
}


void deproteger(int numcase) {

	noyau[VALIDE(numcase)].protect = 0;
}


/* instruction MOV */

/***********************************************
 * copier une operation dans le noyau          *
 *                                             *
 * entree : une operation,                     *
 *          l'adresse de la case cliente       *
 *                                             *
 *                                             *
 * sortie : adresse suivante modulo            *
 ***********************************************/

int copieroper(operation *oper, int loc) {

	noyau[VALIDE(loc)] = *oper;
	return VALIDE(++loc);
}


/***********************************************
 * copier une case dans une autre              *
 *                                             *
 * entree : case source, case destination      *
 *          case de l'instruction              *
 *                                             *
 *                                             *
 * sortie : adresse suivante modulo            *
 ***********************************************/

int copiercase(int source, int dest, int locinit) {

	noyau[VALIDE(dest)] = noyau[VALIDE(source)];
	return VALIDE(++locinit);
}


/***********************************************
 * copier une valeur dans le noyau             *
 *                                             *
 * entree : une valeur, une destination        *
 *          l'adresse de l'instruction cliente *
 *                                             *
 *                                             *
 * sortie : adresse suivante modulo            *
 *                                             *
 * NB : la cellule devient propriete de la     *
 * cellule cliente                             *
 ***********************************************/

int copiervaleur(int val, int loc, int locinit) {
      
	noyau[VALIDE(loc)].codop = DAT;
	noyau[VALIDE(loc)].appart = noyau[VALIDE(locinit)].appart;
	noyau[VALIDE(loc)].tabarg[1].adr = noyau[VALIDE(loc)].tabarg[0].adr = IMMEDIAT;
	noyau[VALIDE(loc)].tabarg[1].val = noyau[VALIDE(loc)].tabarg[0].val = val;
	return VALIDE(++locinit);
}


/* instruction ADD, SUB */
/* les tentatives d'ecritures dans une case protogee par le camp adverse */
/* enlenve la protection mais ne change pas le contenu de la case */

int addDAT( int val, int loc, int locinit ) {

	if ( noyau[VALIDE(loc)].protect && noyau[VALIDE(loc)].protect != noyau[VALIDE(locinit)].protect ) {
		deproteger(loc);
		return MAXCASE+2;
	}

	noyau[VALIDE(loc)].tabarg[1].val =
		VALIDE(noyau[VALIDE(loc)].tabarg[0].val += val);

	noyau[VALIDE(loc)].appart = noyau[VALIDE(locinit)].appart;

	return VALIDE(++locinit);
}

int subDAT( int val, int loc, int locinit ) {

	if ( noyau[VALIDE(loc)].protect && noyau[VALIDE(loc)].protect != noyau[VALIDE(locinit)].protect ) {
		deproteger(loc);
		return MAXCASE+2;
	}

	noyau[VALIDE(loc)].tabarg[1].val =
		VALIDE(noyau[VALIDE(loc)].tabarg[0].val -= val);

	noyau[VALIDE(loc)].appart = noyau[VALIDE(locinit)].appart;

	return VALIDE(++locinit);
}


int addADR( int locA, int locB, int locinit ) {

	if ( noyau[VALIDE(locB)].protect && noyau[VALIDE(locB)].protect != noyau[VALIDE(locinit)].protect ) {
		deproteger(locB);
		return MAXCASE+2;
	}

	noyau[VALIDE(locB)].tabarg[1].val =
		noyau[VALIDE(locB)].tabarg[0].val += noyau[VALIDE(locA)].tabarg[0].val;

	noyau[VALIDE(locB)].appart = noyau[VALIDE(locinit)].appart;

	return VALIDE(++locinit);
}

int subADR( int locA, int locB, int locinit ) {

	if ( noyau[VALIDE(locB)].protect && noyau[VALIDE(locB)].protect != noyau[VALIDE(locinit)].protect ) {
		deproteger(locB);
		return MAXCASE+2;
	}

	noyau[VALIDE(locB)].tabarg[1].val =
		noyau[VALIDE(locB)].tabarg[0].val -= noyau[VALIDE(locA)].tabarg[0].val;

	noyau[VALIDE(locB)].appart = noyau[VALIDE(locinit)].appart;

	return VALIDE(++locinit);
}


/******************************************
 * increment et decrement d'une donnee de *
 * DAT                                    *
 *                                        *
 * entree : argument                      *
 *                                        *
 * sortie : (donne op= donnee)%MAXCASE    *
 *          op parmi (+ -)                *
 ******************************************/

int incDAT(int loc, int locinit) {

	if ( noyau[VALIDE(loc)].protect && noyau[VALIDE(loc)].protect != noyau[VALIDE(locinit)].protect ) {
		deproteger(loc);
		return MAXCASE+2;
	}

	noyau[VALIDE(loc)].appart = noyau[VALIDE(locinit)].appart;

	return noyau[VALIDE(loc)].tabarg[1].val =
		VALIDE(++(noyau[VALIDE(loc)].tabarg[0].val));

}

int decDAT(int loc, int locinit) {

	if ( noyau[VALIDE(loc)].protect && noyau[VALIDE(loc)].protect != noyau[VALIDE(locinit)].protect ) {
		deproteger(loc);
		return MAXCASE+2;
	}

	noyau[VALIDE(loc)].appart = noyau[VALIDE(locinit)].appart;

	return noyau[VALIDE(loc)].tabarg[1].val =
		VALIDE(--(noyau[VALIDE(loc)].tabarg[0].val));

}

/*******************************************
 * calcul d'adresse d'un argument          *
 *                                         *
 * entree : argument                       *
 *                                         *
 * sortie : adresse                        *
 *          MAXCASE donnee non DAT         *
 *          MAXCASE+1 addressage incorrect *
 *          MAXCASE+2 case deprotegee      *
 *                    mais non atteinte    *
 *******************************************/

int calcadr(signed numcase, short numarg, char mode) {

#define NOYCOD(case) (noyau[VALIDE(case)].codop)
#define NOYADR(case, arg) (noyau[VALIDE(case)].tabarg[arg].adr)
#define NOYVAL(case, arg) (noyau[VALIDE(case)].tabarg[arg].val)

#define ADR (NOYADR(numcase, numarg))
#define VAL (NOYVAL(numcase, numarg))

	/* pour les modes d'adressage indirects
		seule une donnee (DAT) peut etre adressee */
	if (  ADR == IMMEDIAT )
		if ( NOYCOD(VAL) != DAT && NOYCOD(numcase) != CMP )
			return MAXCASE; /* erreur pas une donne DAT */

	/* calcul */
	switch (ADR) {

	case IMMEDIAT:
		return VALIDE(VAL);

	case RELATIF:
		return VALIDE(numcase + VAL);

	case INDIRECT:
		if ( NOYCOD(numcase + VAL) != DAT )
			return MAXCASE;
		return VALIDE(numcase +	VAL + NOYVAL(VALIDE(numcase + VAL), 0) );

	case PREINC:
		if (mode == NAFFECT)
			return VALIDE(numcase + VAL + NOYVAL(VALIDE(numcase + VAL), 0) );
		else
			return VALIDE(numcase + VAL + incDAT(VALIDE(numcase + VAL), numcase));

	case PREDEC:
		if (mode == NAFFECT)
			return VALIDE(numcase + VAL + NOYVAL(VALIDE(numcase + VAL), 0) );
		else
			return VALIDE (numcase + VAL + decDAT(VALIDE(numcase + VAL), numcase));
	}

	return MAXCASE+1; /* erreur mode d'adressage illegal */
}

