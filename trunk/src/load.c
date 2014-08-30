/******************************************
 * Projet  : corewar                      *
 * Fichier : load.c                       *
 * Chargeur de programmme                 *
 * Auteur  : NVA                          *
 * Date    : 27/11/94                     *
 ******************************************/

#include <stdlib.h>
#include <time.h>

#include "inc/redcodop.h" /* type instruction */
#include "inc/codobj.h" /* type programme */
#include "inc/core.h" /* noyau */
#include "inc/listprog.h" /* type point d'entree */
#include "inc/stat.h"

CO *load(codobj *prog, codobj *pred, Mstat *progstat) {

auto time_t t;
register int proginit = INSTR1(prog); /* premiere instruction executable */
register int debprog = 0; /* position memoire initiale */
register int loc = 0;  /* position memoire courante */

	/* a-til un predecesseur dans le noyau ? */
	if (pred)
		if ( progstat->locinit <= 0 || progstat->locinit < NBINSTR(pred) || progstat->locinit > MAXCASE - NBINSTR(prog) ) {
		/* charger au hasard */
			srand( (unsigned) time(&t) );
			loc = progstat->locinit = debprog = (rand()%(MAXCASE - NBINSTR(pred) + NBINSTR(prog) + 1) + NBINSTR(pred))%MAXCASE;
		}
		else 
		/* charger a la case indiquee */
			loc = debprog = progstat->locinit;

	/* le premier a etre pose */
	else
		loc = debprog = progstat->locinit;

	/* copie avec transmission de l'identite appart, depuis la tete */
	for ( APPART(prog->suiv) = APPART(prog), prog = prog->suiv; prog; prog = prog->suiv) {
		loc = copieroper(&prog->instr, loc);
		if (prog->suiv) APPART(prog->suiv) = APPART(prog);
	}

	/* retroune la case ou commence le programme */
	return COcreer ((debprog+proginit-1)%MAXCASE);
}
