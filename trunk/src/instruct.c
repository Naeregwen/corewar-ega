/****************************
 * Projet  : corewar        *
 * Fichier : instruct.c     *
 * Jeu d'instructions       *
 * Auteur  : NVA            *
 * Date    : 25/11/94       *
 ****************************/

#include <stdlib.h> /* NULL */
#include <string.h>
#include "inc/instruct.h"

/* ranges a l'envers pour avoir le zero */
const char caradr[MAXCODADR+1] = {'>','<','@','#','\0',' '};

/* structure d'une instruction */

const struct {

	const char mnemo[MAXLGMNEMO+1];	/* mnemonique */
	const short minarg; /* nombre d'arguments nessecaires */
	const short maxarg; /* nombre d'arguments maximum */
	/* structure des codes d'adressage autorises pour une instruction */
	const struct {

		const unsigned relatif  : 1;
		const unsigned immediat : 1;
		const unsigned indirect : 1;
		const unsigned predec   : 1;
		const unsigned preinc   : 1;

	} tabadr[MAXARG]; /* liste de modes d'adressages autorises par arg */

/* table des instructions */

} tabinstr[MAXMNEMO+MAXDIRECTIVE] = {

	{"DAT", 1, 2, {{0,1,0,0,0}, {0,1,0,0,0}}},
	{"MOV", 2, 2, {{1,1,1,1,1}, {1,0,1,1,1}}},
	{"ADD", 2, 2, {{1,1,1,1,1}, {1,0,1,1,1}}},
	{"SUB", 2, 2, {{1,1,1,1,1}, {1,0,1,1,1}}},
	{"JMP", 1, 2, {{1,0,1,1,1}, {1,0.1,1,1}}},
	{"JMZ", 2, 2, {{1,0,1,1,1}, {1,0,1,1,1}}},
	{"JMG", 2, 2, {{1,0,1,1,1}, {1,0,1,1,1}}},
	{"DJZ", 2, 2, {{1,0,1,1,1}, {1,0,1,1,1}}},
	{"CMP", 2, 2, {{1,1,1,1,1}, {1,1,1,1,1}}},
	{"SPL", 1, 2, {{1,0,1,1,1}, {1,0,1,1,1}}},
	{"DJN", 2, 2, {{1,0,1,1,1}, {1,0,1,1,1}}},
	{"JMN", 2, 2, {{1,0,1,1,1}, {1,0,1,1,1}}},
	{"PCT", 1, 2, {{1,0,1,1,1}, {1,0,1,1,1}}},
	{"TST", 2, 2, {{1,0,1,1,1}, {1,0,1,1,1}}},
	{"END", 0, 1, {{1,0,0,0,0}, {0,0,0,0,0}}}
};

/***************************************
 * Recherche de l'existence d'un mnemo *
 *                                     *
 * entree : une chaine de caractere    *
 *                                     *
 * sortie : code operation ou -1       *
 ***************************************/

short estmnemo (char *mot) {

register int i; /* indice de mnemo */

	for (i=0; i<MAXMNEMO; i++)
		if ( strcmp(tabinstr[i].mnemo, mot) == 0 )
			return i;
	return -1;
}


/***************************************
 * Recherche de l'existence            *
 * d'une directive                     *
 *                                     *
 * entree : une chaine de caractere    *
 *                                     *
 * sortie : code directive ou -1       *
 ***************************************/

short estdirective (char *mot) {

register int i; /* indice de mnemo */

	for (i=MAXMNEMO; i<(MAXMNEMO+MAXDIRECTIVE); i++)
		if ( strcmp(tabinstr[i].mnemo, mot) == 0 )
			return i;
	return -1;
}

/***************************************
 * Recherche de l'existence            *
 * d'une instruction                   *
 *                                     *
 * entree : une chaine de caractere    *
 *                                     *
 * sortie : code instruction ou -1     *
 ***************************************/

short estinstruction (char *mot) {

register int i; /* indice de mnemo */

	for (i=0; i<(MAXMNEMO+MAXDIRECTIVE); i++)
		if ( strcmp(tabinstr[i].mnemo, mot) == 0 )
			return i;
	return -1;
}


/**************************************************
 * Recherche de l'existence d'un code d'adressage *
 *                                                *
 * entree : un caractere                          *
 *                                                *
 * sortie : code d'adressage ou -1                *
 **************************************************/

short estcodadr (char c) {

register int i; /* indice de code */

	for (i=0; i<MAXCODADR; i++)
		if ( caradr[i] == c )
			return MAXCODADR-1-i; /* le relatif c'est zero */
	return -1;
}


/***************************************************************
 * Verifie qu'un mode d'adressage est valide                   *
 *                                                             *
 * entree : numero de mnemo, numero d'argument, numero de mode *
 *                                                             *
 * sortie : 0 ou 1                                             *
 ***************************************************************/

short estmodadr(int codadr, int numarg, char codop) {

	switch (codadr) {

	case RELATIF:
		return (short) tabinstr[(int) codop].tabadr[numarg].relatif;
	case IMMEDIAT:
		return (short) tabinstr[(int) codop].tabadr[numarg].immediat;
	case INDIRECT:
		return (short) tabinstr[(int) codop].tabadr[numarg].indirect;
	case PREDEC:
		return (short) tabinstr[(int) codop].tabadr[numarg].predec;
	case PREINC:
		return (short) tabinstr[(int) codop].tabadr[numarg].preinc;
	default:
		return 0;
	}
} 


/**************************************************
 * Lecture du nombre maximal d'argument pour une  *
 * operation                                      *
 *                                                *
 * entree : un code operation                     *
 *                                                *
 * sortie : nombre maximal ou 0                   *
 **************************************************/

short maxarg(char codop) {

	if ( codop < (MAXMNEMO+MAXDIRECTIVE) )
		return tabinstr[(int) codop].maxarg;

	return 0;
}

/**************************************************
 * Lecture du nombre minimal d'argument pour une  *
 * operation                                      *
 *                                                *
 * entree : un code operation                     *
 *                                                *
 * sortie : nombre minimal ou 0                   *
 **************************************************/

short minarg(char codop) {

	if ( codop < (MAXMNEMO+MAXDIRECTIVE) )
		return tabinstr[(int) codop].minarg;

	return 0;
}

/* ****************************************
 * retourne un pointeur sur un mnemonique *
 *                                        *
 * entree : code mnemo                    *
 *                                        *
 * sortie : pointeur                      *
 ******************************************/

char *nommnemo(char codop) {

	if ( codop < MAXMNEMO )
		return (char *) &tabinstr[(int) codop].mnemo;

	return NULL;
}

/* *******************************************
 * retourne un caractere de mode d'adressage *
 *                                           *
 * entree : code mnemo                       *
 *                                           *
 * sortie : un caractere                     *
 *********************************************/

char symboleadr(char modadr) {

	if ( modadr && modadr < MAXCODADR )
		return caradr[MAXCODADR-1-modadr];

	return caradr[MAXCODADR];

}
