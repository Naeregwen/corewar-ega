/*
 ============================================================================
 Name        : cw.cpp
 Author      : cw
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

/******************************************
 * Projet  : corewar                      *
 * Fichier : mars.c                       *
 * Programme principal                    *
 ******************************************/

#include <stdlib.h>
#include <signal.h>

#include "inc/redcodop.h" /* type operation */
#include "inc/listprog.h" /* type programme */
#include "inc/msgerr.h"  /* messages d'execution */
#include "inc/traitarg.h" /* traitement des arguments de la ligne de commande */
#include "inc/tradfic.h" /* traducteur de fichier */
#include "inc/core.h" /* noyau de combat */
#include "inc/load.h" /* chargeur de programme */
#include "inc/exec.h" /* executeur d'instruction */
#include "inc/stat.h" /* statistiques */

void stop (int sig) {

	msgfin();
	errdirecte(MSGBREAK, NULL);
	ferme_sortie();
	exit(1);
}


int main(int argc, char **argv) {

auto codobj *prog1 = NULL;
auto codobj *prog2 = NULL;

auto CO *oper1 = NULL;
auto CO *oper2 = NULL;
auto Mstat *stat1 = NULL;
auto Mstat *stat2 = NULL;

register arglig *args = NULL;
register int cycle = 0;
register int combat = 1;
register int camp = 1; /* 1 = camp1 , 2 = camp2 */

	signal(SIGINT, stop);

	if ( argc == 1 )
		return usage(*argv);

	if ( (args = argligcreer()) == NULL)
		return errdirecte(MEMNONDISPO, NULL);

	if ( !traitarg(argc, argv, args) )
		return usage(*argv);

	/* affichage */
	msginit();

	/* traduction des fichiers et numero du camp */
	if ( !args->nomprog1 || (prog1 = tradfic(args->nomprog1, camp = 1)) == 0 )
		return msgtrad(TRADIMPOSSIBLE, args->nomprog1, camp);
	else
		APPART(prog1) = camp;

	if ( args->nomprog2 ) {		
		if ( (prog2 = tradfic(args->nomprog2, ++camp)) == 0 )
			return msgtrad(TRADIMPOSSIBLE, args->nomprog2, camp);
		else
			APPART(prog2) = camp;
	}

	/* verifier la longueur des deux programmes */
	if (prog2 && (NBINSTR(prog1)+NBINSTR(prog2)) > MAXCASE )
		return errdirecte(TROPDECODE, NULL);

	/* verifier la position des deux programmes */
	if (prog2 && args->pos2 && (0 <= args->pos2 && args->pos2 <= (NBINSTR(prog1)-1)) )
		return errdirecte(MAUVAISEPOS, NULL);

	/* creer le noyau de combat */
	if ( !noyaucreer() )
		return errdirecte(MEMNONDISPO, NULL);

	/* creer les tableaux de stats */
	if ( !(stat1 = Mstatcreer()) )
		return errdirecte(MEMNONDISPO, NULL);
	if ( prog2 )
		if ( !(stat2 = Mstatcreer()) )
			return errdirecte(MEMNONDISPO, NULL);
		else
			stat2->locinit = args->pos2;
	else
		maj_msg(1);

	/* nombre de combats */
	for ( combat =1 ; combat <= args->test; combat++) {

		msgnoyau();

		/* charger les programmes */
		msgtrad(CHARGEMENT, args->nomprog1, 1);
		oper1 = load(prog1, NULL, stat1);
		msgintro(combat,1);
		if (prog2) {
			msgtrad(CHARGEMENT, args->nomprog2, 2);
			oper2 = load(prog2, prog1, stat2);
			msgintro(combat,2);
		}

		/* execution patargee */
		for ( cycle = 0; cycle < args->cyclelim && oper1; cycle++ )
			for ( camp = 1; camp < 3 ; camp++)
				if (camp == 1)
					while (oper1)	 /* tant qu'aucune instruction n'est executee */
						if ( (oper1 = exec(oper1, stat1, camp, args->evol)) == NULL )
							msgperte(MORT, args->nomprog1, camp);
						else {
							if (args->evol && appart(oper1->val) && appart(oper1->val) != camp) {
								COechanger(&oper1, &oper2);
								msgperte(PERTE, args->nomprog1, camp);
								if (!oper1) msgperte(MORT, args->nomprog1, camp);
							}
							break;
						}
				else
					if (oper2) {						
						while (oper2)
							if ( (oper2 = exec(oper2, stat2, camp, args->evol)) == NULL ) {
								oper1 = COdetruire(oper1);
								msgperte(MORT, args->nomprog2, camp);
							}
							else {
								if (args->evol && appart(oper2->val) && appart(oper2->val) != camp) {
									COechanger(&oper2, &oper1);
									msgperte(PERTE, args->nomprog2, camp);
									if (!oper2) {
										oper1 = COdetruire(oper1);
										msgperte(MORT, args->nomprog2, camp);
									}
								}
								break;
							}
					}
	
		/* affichage et remise a zero */
		if (oper1 && oper2) {
			msgperte(NUL, args->nomprog1, 1);
			msgperte(NUL, args->nomprog2, 2);
		}
		msgstat(stat1, args->nomprog1, 1);
		Mstateffacer(stat1);
		if (prog2) {
			msgstat(stat2, args->nomprog2, 2);
			Mstateffacer(stat2);
		}
	}

	msgfin();
	ferme_sortie();

	/* liberer les ressources */
	(void) COdetruire(oper1);
	(void) COdetruire(oper2);
	(void) codobjdetruire(prog1);
	(void) codobjdetruire(prog2);
	Mstatdetruire(stat1);
	Mstatdetruire(stat2);
	noyaudetruire();
	argligdetruire(args);

	return 0;

}
