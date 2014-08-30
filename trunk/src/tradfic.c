/******************************************
 * Projet  : corewar                      *
 * Fichier : tradfic.c                    *
 * Traduction d'un fichier source redcode *
 * Auteur  : NVA                          *
 * Date    : 26/11/94                     *
 ******************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "inc/dicoetiq.h" /* les etiquettes sont rangees dans un dictionnaire */
#include "inc/dicoires.h" /* celles non trouvees lors de la lecture, dans ce 2nd */
#include "inc/tradlig.h" /* traduire une ligne */
#include "inc/instruct.h" /* le nombre maximum d'arguments par instruction */
#include "inc/msgerr.h" /* messages d'erreurs */
#include "inc/codobj.h" /* stocker la traduction */
#include "inc/core.h" /* taille du noyau */

enum { lecterr, lectfich, lectref };

/******************************************
 * traduction d'un fichier source redcode *
 *                                        *
 * entree : un nom de fichier             *
 *                                        *
 * sortie : une liste de code objet       *
 ******************************************/

codobj *tradfic (char *nfic, int numcamp) {

#define CODINSTR (instr->instr.codop)
#define ARGINSTR(n) (instr->instr.tabarg[n].val)

/* lecture */

#define EXTSEPAR   '.' /* separateur d'extension */
#define EXTREDCODE ".rc" /* extension des fichiers redcode */

#define MAXLIG 1024
auto char *s = NULL; /* tampon de lecture des lignes */

register int etatlect = lectfich;

auto FILE *f = NULL; /* descripteur du fichier a lire */
register int numl = 1; /* numero de ligne source */
register int numinstr = 1; /* numero d'instruction traduite */
register int numarg = 0; /* numero d'argument analyse */

/* traduction */

/* lien sur la liste resultant de la traduction
 * la premiere "fausse" cellule permet d'eviter les cas de code de
 * longueur nul.
 * Elle sera changee en END puis recevra des infos sur le programme
 * qu'elle precede  :
 * - nombre d'instructions (DAT compris, pas END)
 * - premiere instruction a executer (point d'entree) */
register codobj *listecode = codobjcreer(DAT);

auto codobj *instr = 0; /* resultat de la traduction d'une instruction */

	/* mises en forme des nom de programme     */
	/* ajouter l'extension ".rc" si pas de '.' */
	if ( (strrchr(nfic, EXTSEPAR)) == NULL )
		strcat(nfic, EXTREDCODE);

	if ( !(f = fopen(nfic,"rb")) ) {
		etatlect = msgtrad(FICHIERILLISIBLE, nfic, numcamp);
		return NULL;
	}

	if ( (s = (char *) calloc (1, MAXLIG+1)) == NULL ) {
		etatlect = msgtrad(MEMNONDISPO, NULL, numcamp);
		return NULL;
	}

	/* creer les dictionnaires pour la traduction */
	if ( !dicoetiqcreer() ) {
		free(s);
		etatlect = msgtrad(MEMNONDISPO, NULL, numcamp);
		return NULL;
	}

	if ( !dicoirescreer() ) {
		free(s);
		dicoetiqdetruire();
		etatlect = msgtrad(MEMNONDISPO, NULL, numcamp);
		return NULL;
	}

	/* lecture du fichier, creation du code, */
	/* et resolution des references arrieres */
	msgtrad(TRADCOMMENCE, nfic, numcamp);

	for (numl=numinstr=1; fgets(s, MAXLIG, f) &&
			/* ignorer la fin du fichier si "END" lu */
			(!listecode || TETEPROG(listecode) != END); numl++)
		/* traduction */
		if ( ( instr = tradlig(s, numl, numinstr, MAXCASE, listecode)) != NULL )
			if ( instr != listecode )

				/* instruction */
				if ( CODINSTR == END ) {

					/* remplacer le DAT de tete par END */
					TETEPROG(listecode) = END;

					/* calcul du debut du programme */
					INSTR1(listecode) = ARGINSTR(0);

					/* effacer le dernier recu */
					(void) codobjeffacer(instr);

				}

				else {
					listecode = codobjchainer(instr, listecode);
					numinstr++;
				}

			/* ligne vide ou etiquette simple */
			else
				continue;

		/* erreur */
		else
			etatlect = errtrad(s, numcamp);

	fclose(f);

	/* longueur du code */
	NBINSTR(listecode) = --numinstr; /* nombre d'instructions */
	if ( numinstr >= MAXCASE )
		etatlect = msgtrad(CODETROPLONG, nfic, numcamp);

	/* ne pas depasser les limites */
	if ( TETEPROG(listecode) == END && NBINSTR(listecode) == 0 )
		etatlect = msgtrad(LIMITEPROG, nfic, numcamp);

	free(s); /* inutile apres */

	/* point d'entree du programme                              *
	 * le point d'entree est, par defaut,                       *
	 * la premiere instruction executable de la liste (non DAT) *
	 * sinon c'est l'instruction dont le numero est donne       *
	 * par la cellule END placee en tete de liste               */

	if ( TETEPROG(listecode) != END )
		/* creer le END et le placer en tete */
		TETEPROG(listecode) = END;

	if ( etatlect && listecode->suiv )
		/* si entree de programme non trouvee */
		if ( INSTR1(listecode) == 0 ) {

			/* chercher la premiere instruction non DAT */
			for ( instr = listecode->suiv, numl=1 ; etatlect && instr && CODINSTR == DAT;
					numl++, instr = instr->suiv)
				if ( !instr->suiv && CODINSTR == DAT)
					etatlect = msgtrad(SEULEMENTDESDAT, nfic, numcamp);

			INSTR1(listecode) = numl; /* premiere instruction executable */
		}

	/* resolution des references avant */
	if ( etatlect && listecode->suiv )
		for ( etatlect = lectref, instr=listecode->suiv, numinstr=1; instr ; numinstr++, instr = instr->suiv)
			for ( numarg = 0; numarg < MAXARG; numarg++)
				if ( instr->argires[numarg] ) {					
					if ( (numl = etiqexist(instr->argires[numarg])) == 0 )
						etatlect = msgtrad(ARGINCONNU, instr->argires[numarg], numcamp);
					else
						ARGINSTR(numarg) = VALIDE(numl - numinstr);
				}


	/* mises en forme des nom de programme */
	/* effacer l'extension ".rc" pour le nom du programme */
	if (strcmp(EXTREDCODE, strrchr(nfic, EXTSEPAR)) == 0 )
		*(strrchr(nfic, EXTSEPAR)) = 0;

	if ( etatlect )
		result(nfic, NBINSTR(listecode), INSTR1(listecode), numcamp);
	else
		listecode = codobjdetruire(listecode);

	/* detruire les dictionnaires */
	dicoetiqdetruire();
	dicoiresdetruire();

	return listecode;

}


