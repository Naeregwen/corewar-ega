/******************************************
 * Projet  : corewar                      *
 * Fichier : stat.h                       *
 * Statistiques des executions            *
 * Auteur  : NVA                          *
 * Date    : 04/12/94                     *
 ******************************************/

#ifndef _STATMARS_H_
#define _STATMARS_H_

typedef struct {

	int locinit; /* position initiale */

	int nbinstr; /* nombre d'instructions executees */
	int nbspl; /* nombre d'instructions crees */
	int nbpct; /* nombre de tentative d'ecriture dans une case protegee */
	int nbnop; /* nombre de blessures (non-operations) */
	int nbcoup; /* nombre d'instruction touche par le camp adverse */
	int nbperte; /* nombre d'instructions perdues par non-execution */
	int nbmute; /* nombre d'instructions perdues par evolution */

} Mstat;

enum { STINSTR, STSPL, STPCT, STNOP, STCOUP, STPERTE, STMUTE};


Mstat *Mstatcreer(void);
void Mstatdetruire(Mstat *elem);
void Mstateffacer(Mstat *elem);
short incstat(Mstat *statis, int lequel);

#endif
