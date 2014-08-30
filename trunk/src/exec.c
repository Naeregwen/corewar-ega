/******************************************
 * Projet  : corewar                      *
 * Fichier : exec.c                       *
 * Executeur d'instructions               *
 ******************************************/

#include "inc/msgerr.h"
#include "inc/redcodop.h" /* type operation */
#include "inc/instruct.h"
#include "inc/core.h"
#include "inc/listprog.h"
#include "inc/stat.h"

CO *exec(CO *proginstr, Mstat *progstat, int numcamp, char modevol) {

#define NUMCASE (proginstr->val)

auto int casetmp = NUMCASE;
auto int valA, valB;
register int AdrA = 0; /* etat initial */
register int AdrB = 0;
register int nouvinstr = MAXCASE; /* par defaut, l'intruction est morte */

	switch ( codeoper(NUMCASE) ) {

	case DAT: /* impossible d'executer */

		incstat(progstat, STPERTE); /* une instruction de perdue */		
		break;

	case MOV:

		if ( modeadr(NUMCASE, 0) == IMMEDIAT )
			if ( (AdrB = calcadr(NUMCASE,1, AFFECT)) < MAXCASE ) {
				/* tentative d'ecriture */
				if ( protect(AdrB) && protect(AdrB) != protect(NUMCASE) ) {
					incstat(progstat, STPCT);
					deproteger(AdrB);
					nouvinstr = VALIDE(NUMCASE+1);
				}
				else {
				/* ecriture */
					if (appart(AdrB) && appart(AdrB) != appart(NUMCASE))
						incstat(progstat, STCOUP);
					nouvinstr = copiervaleur( valarg(NUMCASE,0), AdrB, NUMCASE );
				}
			}
			else
				break;
		else
			if ( (AdrA = calcadr(NUMCASE,0, AFFECT)) < MAXCASE &&
					(AdrB = calcadr(NUMCASE,1, AFFECT))  < MAXCASE ) {
				/* tentative d'ecriture */
				if ( protect(AdrB) && protect(AdrB) != protect(NUMCASE) ) {
					incstat(progstat, STPCT);
					deproteger(AdrB);
					nouvinstr = VALIDE(NUMCASE+1);
				}
				else {
				/* ecriture */
					if (appart(AdrB) && appart(AdrB) != appart(NUMCASE))
						incstat(progstat, STCOUP);
					nouvinstr = copiercase( AdrA, AdrB, NUMCASE );
				}
			}
		break;

	case ADD:
	case SUB:

		if ( modeadr(NUMCASE, 0) == IMMEDIAT )
			if ( (AdrB = calcadr(NUMCASE,1, AFFECT)) < MAXCASE )
				if (codeoper(NUMCASE) == ADD )
					/* tentative d'ecriture */
					if ( protect(AdrB) && protect(AdrB) != protect(NUMCASE) ) {
						incstat(progstat, STPCT);
						deproteger(AdrB);
						nouvinstr = VALIDE(NUMCASE+1);
					}
					/* ecriture */
					else
						nouvinstr = addDAT( valarg(NUMCASE,0), AdrB, NUMCASE );
				else
					/* tentative d'ecriture */
					if ( protect(AdrB) && protect(AdrB) != protect(NUMCASE) ) {
						incstat(progstat, STPCT);
						deproteger(AdrB);
						nouvinstr = VALIDE(NUMCASE+1);
					}
					/* ecriture */
					else
						nouvinstr = subDAT( valarg(NUMCASE,0), AdrB, NUMCASE );
			else
				break;
		else
			if ( (AdrA = calcadr(NUMCASE,0, AFFECT)) < MAXCASE &&
					(AdrB = calcadr(NUMCASE,1, AFFECT))  < MAXCASE ) {				
				if ( codeoper(NUMCASE) == ADD )
					/* tentative d'ecriture */
					if ( protect(AdrB) && protect(AdrB) != protect(NUMCASE) ) {
						incstat(progstat, STPCT);
						deproteger(AdrB);
						nouvinstr = VALIDE(NUMCASE+1);
					}
					else
					/* ecriture */
						if ( codeoper(AdrA) == DAT)
							AdrA = MAXCASE;
						else
							nouvinstr = addADR( AdrA, AdrB, NUMCASE );
				else
					/* tentative d'ecriture */
					if ( protect(AdrB) && protect(AdrB) != protect(NUMCASE) ) {
						incstat(progstat, STPCT);
						deproteger(AdrB);
						nouvinstr = VALIDE(NUMCASE+1);
					}
					else
					/* ecriture */
						if ( codeoper(AdrA) == DAT)
							AdrA = MAXCASE;
						else
							nouvinstr = subADR( AdrA, AdrB, NUMCASE );

			}
		break;

	case CMP:

			if (modeadr(NUMCASE,0) == IMMEDIAT)
				valA = valarg(NUMCASE,0);
			else
				if ( (AdrA = calcadr(NUMCASE,0, AFFECT)) < MAXCASE)
					valA = valarg(AdrA,0);
				else
					break;

			if (modeadr(NUMCASE,1) == IMMEDIAT)
				valB = valarg(NUMCASE,1);
			else
				if ( (AdrB = calcadr(NUMCASE,1, AFFECT)) < MAXCASE)
					valB = valarg(AdrB,1);
				else
					break;

			if (valA == valB)
				nouvinstr = VALIDE(NUMCASE+2);
			else
				nouvinstr = VALIDE(NUMCASE+1);

		break;

	case TST:
		
		if ( (AdrA = calcadr(NUMCASE,0, AFFECT)) < MAXCASE ) {			
			if (protect(AdrA) && protect(AdrA) != protect(NUMCASE) )
				nouvinstr = VALIDE(NUMCASE+1);
			else
				nouvinstr = VALIDE(NUMCASE+2);
		}
		break;

	case JMP:

		if ( modeadr(NUMCASE, 0) != IMMEDIAT ) {			
			if ( (AdrA = calcadr(NUMCASE,0, AFFECT)) < MAXCASE )
				nouvinstr = AdrA;
			else
				nouvinstr = VALIDE(NUMCASE+1);
		}
		break;

	case JMZ:

		if ( modeadr(NUMCASE, 0) != IMMEDIAT && modeadr(NUMCASE, 1) != IMMEDIAT )
			if ( (AdrA = calcadr(NUMCASE,0, AFFECT)) < MAXCASE && (AdrB = calcadr(NUMCASE,1, AFFECT)) < MAXCASE ) {				
				if ( valarg(AdrB, 0) == 0 )
					nouvinstr = AdrA;
				else
					nouvinstr = VALIDE(NUMCASE+1);
			}
		break;

	case JMN:

		if ( modeadr(NUMCASE, 0) != IMMEDIAT && modeadr(NUMCASE, 1) != IMMEDIAT )
			if ( (AdrA = calcadr(NUMCASE,0, AFFECT)) < MAXCASE && (AdrB = calcadr(NUMCASE,1, AFFECT)) < MAXCASE ) {				
				if ( valarg(AdrB, 0) != 0 )
					nouvinstr = AdrA;
				else
					nouvinstr = VALIDE(NUMCASE+1);
			}
		break;

	case JMG:

		if ( modeadr(NUMCASE, 0) != IMMEDIAT && modeadr(NUMCASE, 1) != IMMEDIAT )
			if ( (AdrA = calcadr(NUMCASE,0, AFFECT)) < MAXCASE && (AdrB = calcadr(NUMCASE,1, AFFECT)) < MAXCASE ) {				
				if ( valarg(AdrB, 0) > 0 )
					nouvinstr = AdrA;
				else
					nouvinstr = VALIDE(NUMCASE+1);
			}
		break;

	case DJZ:

		if ( modeadr(NUMCASE, 0) != IMMEDIAT && modeadr(NUMCASE, 1) != IMMEDIAT )
			if ( (AdrA = calcadr(NUMCASE,0, AFFECT)) < MAXCASE && (AdrB = calcadr(NUMCASE,1, AFFECT)) < MAXCASE ) {
				if ( decDAT(AdrB, NUMCASE) == 0 )
					nouvinstr = AdrA;
				else
					nouvinstr = VALIDE(NUMCASE+1);
			}
		break;

	case DJN:

		if ( modeadr(NUMCASE, 0) != IMMEDIAT && modeadr(NUMCASE, 1) != IMMEDIAT )
			if ( (AdrA = calcadr(NUMCASE,0, AFFECT)) < MAXCASE && (AdrB = calcadr(NUMCASE,1, AFFECT)) < MAXCASE ) {				
				if ( decDAT(AdrB, NUMCASE) != 0 )
					nouvinstr = AdrA;
				else
					nouvinstr = VALIDE(NUMCASE+1);
			}
		break;

	case SPL:

		if ( modeadr(NUMCASE, 0) != IMMEDIAT )
			if ( (AdrA = calcadr(NUMCASE,0, AFFECT)) < MAXCASE ) {
				/* creation d'un nouveau point d'entree */
				proginstr = COchainer (COcreer(AdrA), proginstr);
				incstat(progstat, STSPL);
				nouvinstr = VALIDE(NUMCASE+1);

			}
		break;
	
	case PCT:
		
		if ( modeadr(NUMCASE, 0) != IMMEDIAT )
			if ( (AdrA = calcadr(NUMCASE,0, AFFECT)) < MAXCASE ) {
				if ( !protect(AdrA) && appart(AdrA) == appart(NUMCASE) ) {
					incstat(progstat, STPCT);
					proteger(AdrA, numcamp);
				}
				nouvinstr = VALIDE(NUMCASE+1);

			}

	default: /* impossible d'executer */

		break;
	}

	if (nouvinstr >= MAXCASE || AdrA >= MAXCASE || AdrB >= MAXCASE ) {

		/* on est tombe sur une instruction inexecutable */
		if (nouvinstr == MAXCASE) {
			if ( codeoper(NUMCASE) == DAT )
				msginstr(NUMCASE, codeoper(NUMCASE), modeadr(NUMCASE, 0),
					valarg(NUMCASE,0), modeadr(NUMCASE, 1), valarg(NUMCASE,1), numcamp);
			msgexe(NONEXE, NUMCASE, numcamp);
		}

		/* l'instruction etait executable mais comportait une erreur */
		if ( AdrA == MAXCASE || AdrB == MAXCASE ) {
			msgexe(DATSEULEMENT, NUMCASE, numcamp);
			nouvinstr = VALIDE (++nouvinstr);
		}
	
		if ( AdrA == MAXCASE+1 || AdrB == MAXCASE+1 ) { 
			msgexe(MODADRILLEGAL, NUMCASE, numcamp);
			nouvinstr = VALIDE (++nouvinstr);
		}
	
		if ( AdrA == MAXCASE+2 || AdrB == MAXCASE+2 ) {
			msgexe(CASEPROTEGEE, NUMCASE, numcamp);
			nouvinstr = VALIDE (++nouvinstr);
		}
	
		proginstr = COeffacer(proginstr);
		msgexe (MORTE, casetmp, numcamp);
		return proginstr;
	}

	incstat(progstat, STINSTR);

	/* message 1 */

	msginstr(NUMCASE, codeoper(NUMCASE), modeadr(NUMCASE, 0), valarg(NUMCASE,0),
		modeadr(NUMCASE, 1), valarg(NUMCASE,1), numcamp);

	msgpoint(NUMCASE);
	if ( codeoper(NUMCASE) == MOV || codeoper(NUMCASE) == ADD || codeoper(NUMCASE) == SUB )
			msgpoint(AdrB);

	proginstr->val = nouvinstr;

	/* message 2 */
	/* evolution */
	if (modevol && appart(NUMCASE) && appart(NUMCASE) != numcamp) {

		msgexe(EVOLUTION, NUMCASE, numcamp);
		incstat(progstat, STMUTE);
		return proginstr;
	}

	return proginstr->suiv;
}

