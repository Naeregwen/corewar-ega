/*****************************************
 * Projet  : corewar                     *
 * Fichier : tradlig.c                   *
 * Lecture d'une ligne source de corewar *
 * Auteur  : NVA                         *
 * Date    : 20/11/94                    *
 *****************************************/

/***************************************************************************
 * Structure d'une ligne source                                            *
 *                                                                         *
 * [etiquette]  mnemo [cod1] adr1 [, [code2] adr2 [; [comment]]]           *
 * "etiquette" commence par un alphabetique et compose d'alphanumeriques   *
 * "mnemo" est l'un de ceux definis                                        *
 * "cod" est un des codes d'adressage                                      *
 * "adr" est une adresse sous forme numerique ou alphanumerique(etiquette) *
 *                                                                         *
 ***************************************************************************/

#include <stdlib.h> /* calloc(), free() */
#include <stdio.h> /* sscanf() */
#include <ctype.h> /* is...() */
#include <string.h> /* memeset() */

#include "inc/dicoetiq.h" /* les etiquettes sont rangees dans un dictionnaire */
#include "inc/dicoires.h" /* celles non trouvees lors de la lecture, dans ce 2nd */
#include "inc/instruct.h" /* le nombre maximum d'arguments par instruction */
#include "inc/msgerr.h" /* erreur de traduction */
#include "inc/codobj.h" /* stocker la traduction */

/* etat d'avancement de la lecture */
enum {lecterr, lectetiq, lectmnemo, lectcode,
	lectarg, lectvirg, lectcom, findl};

/* separateurs de champs */

#define COMMENT ';' /* commentaires */
#define VIRG ',' /* arguments */
#define PLUS '+'
#define MOINS '-'

/********************************************************/
/* traduction d'une ligne source                        */
/*                                                      */
/* entree : une chaine de caracteres                    */
/*          un numero de ligne                          */
/*          une liste de code objets                    */
/*                                                      */
/* sortie : la meme liste : lecture mais pas traduction */
/*          le nouvel element : lecture et traduction   */
/*          NULL : erreur                               */
/********************************************************/

codobj *tradlig (char *ligne, int numl, int numinstr, int max, codobj *listecode) {

#define MAXLGETIQ 10 /* longueur max d'une etiquette */
#define MAXLGARG 10 /* longueur max d'un argument */

#define ISWHT(c) (isspace(c) && c != '\n')
#define ISBRK(c) (!c || c == '\n' || c == COMMENT)
#define ISSIG(c) ( c == PLUS || c == MOINS)
#define ISNUM(c) (isdigit(c) || ISSIG(c))
#define ISARG(c) (isalnum(c) || ISSIG(c))

#define ETATARG(c) (ISBRK(c)? findl : (c == VIRG)? lectcode : lectvirg)

#define VALCOD (ligobj->instr.codop)
#define VALADR(n) (ligobj->instr.tabarg[n].adr)
#define VALARG(n) (ligobj->instr.tabarg[n].val)
#define VALIRES(n) (ligobj->argires[n])

#define ERREUR(cod, arg) enerrtrad(cod, numl, ligne, arg)

register int etatlect = lectetiq;  /* chercher d'abord une etiquette */
auto char *mnemo=NULL;            /* espace de stockage du mnemo lu */
register char *mnemolu = mnemo;  /* pointeur dans le mnemo */
auto char *etiq=NULL;           /* espace de stockage de l'etiquette lue */
register char *etiqlu = etiq;  /* pointeur dans l'etiquette */
auto char *arg=NULL;          /* espace de stockage de l'argument lu */
register char *arglu= arg;   /* pointeur dans l'argument */
register codobj *ligobj=0;  /* lien sur la nouvelle operation traduite */
auto int ligetiq=0;        /* pointeur d'etiquette du dico */
auto int numarg=0;    /* numero de l'argument attendu (1er ou 2nd) */
auto int codadr=0;   /* correspondance du mode d'adressage */
auto char codop=0;  /* code operation du mnemo lu */


	/* initialiser les tableaux et pointeurs */
	if ( (mnemo = (char *) calloc (MAXLGMNEMO+1, sizeof(char))) == NULL
			|| (etiq = (char *) calloc (MAXLGETIQ+1, sizeof(char))) == NULL
			|| (arg = (char *) calloc (MAXLGARG+1, sizeof(char))) == NULL ) {
		etatlect = ERREUR (MEMNONDISPO, NULL);
		return NULL;
	}

	mnemolu = mnemo; etiqlu = etiq; arglu = arg;

	numarg = codadr = codop = 0;

	/* lecture caractere par caractere */
	for (; ; *ligne = (*ligne == '\t')? ' ' : *ligne, ligne++)

		switch (etatlect) {


		case lecterr:

			/* liberer les ressources */
			free(etiq); free(mnemo); free(arg);
			if (ligobj) free(ligobj);

			return NULL;

		case lectetiq:

			/* debut d'etiquette */
			/* premier caractere */
			if ( etiqlu == etiq )

				/* si espace chercher un mnemo et non une etiquette	 */
				/* qui doit imperativement commencer en debut de ligne */
				if ( isspace(*ligne) )
					etatlect = lectmnemo; /* pas d'etiquette */

				else

					/* ligne vide */
					if ( ISBRK(*ligne) )
						etatlect = findl;

					/* premier caractere d'une etiquette doit etre alphabetique */
					else
						if ( !isalpha (*ligne) )
							etatlect = ERREUR (ETIQNONALPHA, NULL);

						else
							/* premier caractere du mnemo, tout en MAJ */
							*etiqlu++ = toupper(*ligne);

			/* suite d'etiquette */
			/* caracteres suivants */
			else

				/* suite d'etiquette */
				/* caractere alphanumerique */
				if ( isalnum(*ligne) )

					/* pas plus de MAXLGETIQ caracteres */
					if ( etiqlu-etiq == MAXLGETIQ )
						etatlect = ERREUR (ETIQLONG, NULL);
					else

						/* copie du caractere de l'etiquette tout en MAJ */
						*etiqlu++ = toupper(*ligne);

				/* fin d'etiquette */
				/* caractere non alphanumerique */
				else

					/* espace ou commentaire signale la fin d'une etiquette */
					if ( *ligne && !isspace(*ligne) && *ligne != COMMENT )
						etatlect = ERREUR (ETIQNONALNUM, NULL);

					else
						/* verifier que ce n'est pas un mnemo */
						if ( estmnemo(etiq) != -1 )
							etatlect = ERREUR (ETIQILLEGALE, NULL);

						else
							/* l'etiquette, existe-t-elle deja ? */
							if ( !etiqajouter(etiq, numinstr) )
								etatlect = ERREUR (ETIQEXIST, NULL);

							else
								/* passer au mnemo */
								etatlect = lectmnemo;

			break;

		case lectmnemo:

			/* debut de mnemo */
			/* premier caractere */
			if ( mnemolu == mnemo )

				/* passer les blancs */
				if ( ISWHT(*ligne) )
					continue;

				else
					/* fin de ligne ou commentaires */
					if ( ISBRK(*ligne) )
						etatlect = findl; /* pas de mnemo */

					else
						/* premier caractere de mnemo doit etre alphabetique */
						if ( !isalpha(*ligne) )
							etatlect = ERREUR (MNEMONONALPHA, NULL);

						else
							/* premier caractere du mnemo, tout en MAJ */
							*mnemolu++ = toupper(*ligne);

			/* suite de mnemo */
			/* caracteres suivants */
			else

				/* suite de mnemo */
				/* caracteres alphabetiques */
				if ( isalpha (*ligne) )

					/* trop long pour etre un mnemo */
					if ( mnemolu-mnemo == MAXLGMNEMO )
						etatlect = ERREUR (MNEMOLONG, NULL);

					/* copie du caractere du mnemo, tout en MAJ */
					else
						*mnemolu++ = toupper(*ligne);

				/* fin de mnemo */
				/* caracteres non alphabetiques */
				else
					/* suite(fin) de mnemo inconnue */
					if ( !ISBRK(*ligne) && !isspace(*ligne) &&
							/* permet d'accepter des arguments colles au mnemo */
							/* s'il se distinguent par un signe ou un code */
							/* d'addressage */
							!ISSIG(*ligne) && estcodadr(*ligne)==-1 )
						etatlect = ERREUR (MNEMONONALPHA, NULL);

					/* un espace */
					/* fin de ligne ou commentaires */
					/* ou un debut d'argument */
					else
						if ( (codop = estinstruction (mnemo)) == -1 )
								etatlect = ERREUR (INSTRINCONNU, NULL);

						else
							/* nouveau mnemo */
							if ( !(ligobj = codobjcreer(codop)) )
								etatlect = ERREUR (MEMNONDISPO, NULL);

							else {
								/* etape suivante de lecture */
								ligne--; /* reposer le caractere */
								etatlect=lectcode;
							}

			break;

		case lectcode:

			/* passer les blancs */
			if ( ISWHT(*ligne) )
				continue;

			else

				if ( numarg > maxarg(VALCOD) )
					etatlect = ERREUR (ARGSUPERFLU, NULL);

				else
					if ( ISBRK(*ligne) )
						if ( numarg < minarg(VALCOD) )
							etatlect = ERREUR (ARGMANQUANT, NULL);
						else
							etatlect = findl;
					else
						if ( !ISARG(*ligne) )
							if ( (codadr = estcodadr(*ligne)) == -1 )
								etatlect = ERREUR (ARGILLEGAL, NULL);

							else
								if ( !estmodadr (codadr, numarg, codop) )
									etatlect = ERREUR (MODADRILLEGAL, NULL);

								else {
									VALADR(numarg) = codadr;
									etatlect = lectarg;
								}
						else {
							ligne--; /* reposer le caractere */
							etatlect = lectarg;
						}

			break;

		case lectarg:

			/* debut d'argument */
			/* premier caractere */
			if ( arglu == arg )

				if ( ISWHT(*ligne) )
					continue;

				else
					/* fin de ligne ou commentaires */
					if ( ISBRK(*ligne) )
						if ( numarg < minarg(VALCOD) )
							etatlect = ERREUR (ARGMANQUANT, NULL);
						else
							/* pas d'argument */
							etatlect = findl;

					else
						/* argument alphanumerique */
						if ( !ISARG(*ligne) )
							etatlect = ERREUR (ARGILLEGAL, NULL);

						else
							/* premier caractere de l'argument, tout en MAJ */
							*arglu++ = toupper(*ligne);

			/* suite d'argument */
			/* caracteres suivants */
			else

				/* suite d'argument */
				/* caractere alphanumerique */
				if ( isalnum(*ligne) )
					if ( arglu-arg == MAXLGARG )
						etatlect = ERREUR (ARGLONG, NULL);
					else
						*arglu++ = toupper(*ligne);
				
				/* fin d'argument */
				/* caractere non alphanumerique */
				else
					if ( ISBRK(*ligne) || isspace (*ligne) || *ligne == VIRG ) {

						/* argument numerique */
						if ( ISNUM(*arg) )
							if ( !(sscanf(arg, "%d", &VALARG(numarg))) )
								etatlect = ERREUR (ARGNUMINCORRECT, NULL);
							else
								etatlect = ETATARG(*ligne);

						/* argument alphabetique */
						else
							if ( (ligetiq = etiqexist(arg)) != 0 ) {

								/* resolution des references arrieres */
								/* calcul de la distance qui separe les lignes
									MODULO le nombre de cases */

								/* rencontre de END, donner simplement le numero
								de la ligne */
								if ( VALCOD == END )
									VALARG(0) = VALARG(1) = ligetiq;

								/* calcul normal de la distance */
								else
									VALARG(numarg) = (ligetiq - numinstr)%max;

								etatlect = ETATARG(*ligne);
							}
							else

								/* rencontre de END, fin de traduction */
								if ( VALCOD == END )
									etatlect = ERREUR (FININCONNUE, NULL);
								else
									/* ajouter dans la table des non resolues */
									if ( !(VALIRES(numarg) = ireslier(arg)) )
										etatlect =  ERREUR (MEMNONDISPO, NULL);
									else
										etatlect = ETATARG(*ligne);

						if ( etatlect == lecterr ) continue;

						/* mise en forme des arguments */
						switch (VALCOD) {

						case DAT:

							/* si le mode d'addressage est omis (relatif)
								forcer le mode d'addressage immediat */
							VALADR(numarg) = 1;

						case JMP:
						case SPL:
						case PCT:

							/* si deux arguments different ou premier non nul */
							/* DAT "#nn, #0" ou "#nn, #nn" ou "#0, #nn" */
							if (numarg == 1 )
								if ( VALARG(0) != 0 )
									if ( VALARG(1) != 0 && (VALARG(0) != VALARG(1) ||
											VALADR(0) != VALADR(1)) )
										etatlect = ERREUR (SECONDILLEGAL, NULL);
									else
										break;

								/* copier le 2nd argument a la place du 1er "0" */
								else {
									VALARG(0) =	VALARG(1);
									VALADR(0) = VALADR(1);
								}

							/* un seul argument pour l'instant */
							else {
								/* recopier le 1er argument a la place du 2nd "0" */
								VALARG(1) =	VALARG(0);
								VALADR(1) = VALADR(0);
							}
							break;

						case MOV: case ADD: case SUB:

							break;

						case TST: case CMP:
						case JMZ: case DJZ: case DJN:

							break;

						case END:

							break;
						}

						if (etatlect == lecterr ) continue;

						/* verification des arguments */
						if ( !estmodadr(VALADR(numarg), numarg, VALCOD) )
							etatlect = ERREUR(MODADRILLEGAL, NULL);

						numarg++;

						/* remise a zero de arg et arglu */
						memset(arg, 0, (MAXLGARG+1)*sizeof(char));
						arglu = arg;
					}

			break;

		case lectvirg:

			/* attente d'une virgule */
			/* passer les blancs */
			if ( ISWHT(*ligne) )
				continue;

			else
				/* fin de ligne */
				if ( ISBRK(*ligne) )

					/* verifier que le mode d'addressage
					autorise le nombre d'argument */
					if ( numarg < minarg(VALCOD)  )
						etatlect = ERREUR (ARGMANQUANT, NULL);

					else
						/* valide mais pas complet */
						etatlect = findl;

				/* un caractere */
				else
					if ( *ligne != VIRG )
						if ( !ISARG(*ligne) && !estcodadr(*ligne) )		
							etatlect = ERREUR (VIRGATTENDUE, NULL);
						else {
							/* sauter les virgules quand il n'y en a pas */
							ligne--; /* reposer le caractere */
							etatlect = lectcode;
						}
					else
						etatlect = lectcode;

			break;

		case lectcom:

			/* passer les blancs */
			if ( ISWHT(*ligne) )
				continue;

			/* chercher soit des arguments en trop mais valides */
			/* soit un commentaire -> fin */
			else
				if ( !ISBRK(*ligne) )
					/* fin de ligne anormale */
					etatlect = ERREUR (CARSUPERFLU, NULL);
				else
					/* passer les commentaires */
					etatlect = findl;

			break;

		case findl:

			/* liberer les ressources */
			free(etiq); free(mnemo); free(arg);

			if (!ligobj)
				return listecode;
			else
				return ligobj;

		default: break;

		}


}


