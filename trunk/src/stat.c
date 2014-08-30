/******************************************
 * Projet  : corewar                      *
 * Fichier : stat.c                       *
 * Statistiques des executions            *
 * Auteur  : NVA                          *
 * Date    : 04/12/94                     *
 ******************************************/

#include <stdlib.h>

#include "inc/stat.h"

Mstat *Mstatcreer(void) {

register Mstat *nouv = (Mstat *) calloc (1, sizeof(Mstat));

	if (!nouv) return NULL;
	return nouv;
}


void Mstatdetruire(Mstat *elem) {

	if (elem) free(elem);
}


void Mstateffacer(Mstat *elem) {

	elem->nbinstr = elem->nbspl = elem->nbpct = elem->nbcoup =
	elem->nbcoup = elem->nbperte = elem->nbmute = elem->locinit = 0;
}

short incstat(Mstat *statis, int lequel) {

	switch (lequel) {

	case STINSTR: ++statis->nbinstr; break;
	case STSPL: ++statis->nbspl; break;
	case STPCT: ++statis->nbpct; break;
	case STNOP: ++statis->nbnop; break;
	case STCOUP: ++statis->nbcoup; break;
	case STPERTE: ++statis->nbperte; break;
	case STMUTE: ++statis->nbmute; break;
	default: return 0;
	}
	return 1;
}

