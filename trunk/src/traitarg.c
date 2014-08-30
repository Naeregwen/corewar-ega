/******************************************
 * Projet  : corewar                      *
 * Fichier : traitarg.c                   *
 * Traitement des arguments de la ligne   *
 * de commande                            *
 ******************************************/

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

#include "inc/traitarg.h"
#include "inc/msgerr.h"  /* messages d'execution */
#include "inc/core.h"

arglig *argligcreer(void) {

register arglig *nouv = (arglig *) calloc (1, sizeof(arglig));

	if (!nouv) return NULL;
	nouv->nomprog1 = nouv->nomprog2 = nouv->sortie = NULL;
	nouv->cyclelim = MAXCYCLE;
	return nouv;
}

void argligdetruire(arglig *elem) {

	if (!elem) return;
	if (elem->nomprog1) free(elem->nomprog1);
	if (elem->nomprog2) free(elem->nomprog2);
	if (elem->sortie) free(elem->sortie);
	free(elem);
}


short traitarg(int argc, char **argv, arglig *args) {

register int numarg = 0;

	for (numarg=1; numarg < argc; numarg++)
		switch (*argv[numarg]) {

		/* une option */
		case '-':
			if (strlen(argv[numarg]))
				switch ( toupper(*(argv[numarg]+1)) ) {

				case OPTSORTIE:
	
					if (*(argv[numarg]+2))
						if (!args->sortie && (args->sortie =
								(char *) calloc (MAXNOMFIC, sizeof(char))) == NULL )
							return errdirecte(MEMNONDISPO, argv[numarg]);
						else 
							if ( !maj_sortie( &(*(argv[numarg]+2)) ) )
								errdirecte(MAUVAISFIC, &(*(argv[numarg]+2)) );
							else	
								strcpy(args->sortie, &(*(argv[numarg]+2)));
					else
						errdirecte(OPTVIDE, argv[numarg]);

					break;

				case OPTPOS:

					if (!*(argv[numarg]+2))
						errdirecte(OPTVIDE, argv[numarg]);
					else
						if ( (args->pos2 = atoi(&(*(argv[numarg]+2)))) <= 0 ||
								args->pos2 > MAXCASE)
							errdirecte(MAUVAISEPOS, argv[numarg]);

					break;

				case OPTCYCLIM:
	
					if (!*(argv[numarg]+2))
						errdirecte(OPTVIDE, argv[numarg]);
					else
						if ( (args->cyclelim = atoi(&(*(argv[numarg]+2)))) == 0 ||
								args->cyclelim == INT_MAX)
							errdirecte(MAUVAISELIM, argv[numarg]);
					break;

				case OPTEVOL:

					if ( *(argv[numarg]+2) )
						errdirecte(OPTINCONNUE, argv[numarg]);
					else
						args->evol = 1;
					break;

				case OPTGRAPH:

					if ( !*(argv[numarg]+2) )
						maj_graph(args->graph = GRAPH);
					else  {
						args->graph = atoi(argv[numarg]+2);
						if (args->graph > MIXTE) {
						  errdirecte(OPTINCONNUE, argv[numarg]);
						  args->graph = TEXTE;
						}
					   maj_graph(args->graph);
					}
					break;

				case OPTTRY:
					
					if ( !*(argv[numarg]+2) )
						errdirecte(OPTVIDE, argv[numarg]);
					else
						if ( (args->test = atoi(&(*(argv[numarg]+2)))) == 0 ||
								args->test == INT_MAX)
							errdirecte(MAUVAISELIM, argv[numarg]);
						
					break;

				default:
					errdirecte(OPTINCONNUE, argv[numarg]);
					break;
				}

			break;


		default :
			/* premier nom de programme */
			if (!args->nomprog1) {

				if ( (args->nomprog1 = (char *)
						calloc (MAXNOMFIC, sizeof(char))) == NULL )
					return errdirecte(MEMNONDISPO, argv[numarg]);

				strcpy(args->nomprog1, argv[numarg]);
			}
			/* second nom de programme */
			else
				if (!args->nomprog2) {

					if ( (args->nomprog2 = (char *)
							calloc (MAXNOMFIC, sizeof(char))) == NULL )
						return errdirecte(MEMNONDISPO, argv[numarg]);

					strcpy(args->nomprog2, argv[numarg]);
					maj_msg(2); /* peut-etre deux canmps */
				}
			/* erreur */
				else
					errdirecte(TROPDENOM, argv[numarg]);

			break;

		}

	if ( !args->test )
		args->test = 1; /* au moins un combat */
	return 1;
}
