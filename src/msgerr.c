/*******************************************
 * Projet  : corewar                       *
 * Fichier : msgerr.c                      *
 * Gestion des erreurs d'execution de mars *
 *******************************************/

#include <stdio.h>
#include <string.h>

#include "inc/msgerr.h"
#include "inc/core.h"
#include "inc/instruct.h" /* pour les messages d'execution */
#include "inc/visu.h"

struct {

	const unsigned cardec: 1; /* le curseur doit reculer pour indiquer l'erreur */
	const char *msg; /* message approprie */

} msgMARS[MAXMSG] = {

	{0, "ok"},

	{0, "%s : Traduction du fichier...\n"},
	{0, "Programme %s :\n"},
	{0, "%d instruction(s).\n"},
	{0, "Entree du programme : Instruction %d.\n"},
	{0, "%s : Impossible de traduire.\n"},
	{0, "%s : Chargement du programme\n"},

	{1, "Une etiquette doit commencer par un alphabetique"},
	{0, "Une etiquette ne doit contenir que des alphanumeriques"},
	{0, "Une etiquette trop longue"},
	{1, "Etiquette deja definie"},
	{1, "Nom d'etiquette interdit, c'est un mot reserve"},
	{0, "Un mnemonique ne contient que des alphabetiques"},
	{0, "Symbole trop long pour etre un mnemonique"},
	{1, "Instruction inconnue"},
	{0, "Mode d'addressage illegal"},
	{1, "Argument absent"},
	{1, "Argument incorrect"},
	{1, "Argument illegal"},
	{1, "Argument superflu"},
	{0, "Argument trop long"},
	{1, "Argument numerique incorrect"},
	{0, "Virgule attendue"},
	{0, "Caractere(s) superflu(s) en fin de ligne"},
	{1, "Second argument incorrect (\"#nn, #0\" ou \"#nn, #nn\" ou \"#0, #nn\")"},
	{1, "Argument de END inconnu"},
	{0, "%s : Argument inconnu\n"},

	{1, "%s : Argument de END en dehors du programme\n"},
	{0, "%s : Il manque la directive END. Pas d'entree dans le programme\n"},
	{0, "%s : Ce programme ne contient que des DAT\n"},
	{0, "%s : Fichier vide, sans instructions\n"},
	{0, "%s : Lecture du fichier impossible\n"},
	{0, "%s : Pas assez de place pour loger le programme\n"},
	{0, "Pas assez de place pour loger les deux programmes\n"},

	{0, "Combat numero "},
	{0, "Tentative d'acces a une case protegee"},
	{0, "Reference non DAT illegale."}, /* avec MODADRILLEGAL (nop) */
	{0, "L'instruction a mute."}, /* evolution */
	{0, "Instruction non executable."}, /* perte */
	{0, "L'instruction est morte."}, /* perte */
	{0, " a perdu une instruction."}, /* perte */
	{0, " est mort."}, /* perte */
	{0, " Match nul"},

	{0, "Option inconnue"},
	{0, "Argument d'option manquant"},
	{0, "Pas plus de deux nom de programmes"},
	{0, "Mauvais argument de position"},
	{0, "Mauvais argument de limite d'instructions"},
	{0, "Mauvais nom de fichier ou illisible"},

	{0, "Numero de case initiale : %4d\n"},
	{0, "Nombre d'instruction executees : %4d\n"},
	{0, "Nombre d'acces a une case protege : %4d\n"},
	{0, "Nombre d'instruction crees : %4d\n"},
	{0, "Nombre de blessure par non-operation : %4d\n"},
	{0, "Nombre de coups portes a l'adversaire : %4d\n"},
	{0, "Nombre d'instructions mortes : %4d\n"},
	{0, "Nombre d'instruction perdues par evolution : %4d\n"},

	{0, "Plus de place en memoire"},
	{0, "MARS interrompu\n"}
};


/* sauvegarder le statut de l'erreur de traduction */

static struct etaterr {

	int coderr; /* code d'erreur */
	int lerr; /* ligne incriminee */
	char *carerr; /* caractere incrimine */
	char *argerr; /* argument eventuel au message */

} Errt;

static char modsortie = TEXTE; 
static char nbprog = 1;

/* fichier de sortie pour les resultats */

static FILE *sortie = NULL;

/*****************************/
/* mise en erreur            */
/* et conservation de l'etat */
/*                           */
/* entree : code d'erreur    */
/*          caractere stop   */
/*                           */
/* sortie : etat d'erreur    */
/*****************************/

int enerrtrad(int cod, int lig, char *car, char *arg) {

	Errt.coderr = cod;
	Errt.lerr = lig;
	Errt.carerr = car;
	Errt.argerr = arg;
	return 0;
}

/***************************************************
 * erreurs de traduction                           *
 *                                                 *
 * entree : un code d'erreur, un argument d'erreur *
 *          un numero de ligne                     *
 *                                                 *
 * sortie : un etat d'erreur                       *
 ***************************************************/

int errtrad(char *ligne, int numcamp) {

	/* pas d'erreurs */
	if ( Errt.coderr == 0 )
		return 0;

	if (modsortie == TEXTE ) {

		fprintf(stderr, "%s", ligne);
		/* mise en forme du message */
		if ( msgMARS[Errt.coderr].cardec )
			Errt.carerr--;
	
		for ( ; *ligne && ligne < Errt.carerr; ligne++)
			fprintf(stderr, "%c", ' ');
		fprintf(stderr, "%c", '^');
	
		fprintf (stderr, "\nLigne %d , " , Errt.lerr );
		if (Errt.argerr)
			fprintf(stderr,"%s : %s.\n", Errt.argerr, msgMARS[Errt.coderr].msg);
		else
			fprintf(stderr,"%s.\n", msgMARS[Errt.coderr].msg);
	}
	else {

		affmsg("%s", ligne, numcamp);
		/* mise en forme du message */
		if ( msgMARS[Errt.coderr].cardec )
			Errt.carerr--;
	
		for ( ; *ligne && ligne < Errt.carerr; ligne++)
			affcar(' ', numcamp);
		affcar('^', numcamp);
	
		affint ("\nLigne %d , " , Errt.lerr, numcamp );
		if (Errt.argerr) {
			affmsg("%s : ", Errt.argerr, numcamp);
			affmsg("%s.\n", (char *) msgMARS[Errt.coderr].msg, numcamp);
		}
		else
			affmsg("%s.\n", (char *) msgMARS[Errt.coderr].msg, numcamp);
	}

	/* remise a zero du statut de l'erreur */
	Errt.coderr=0;
	Errt.lerr=0;
	Errt.carerr=0;
	Errt.argerr=0;

	return 0;
}

/***************************************************
 * erreurs de traduction directe                   *
 *                                                 *
 * entree : un code d'erreur, un argument d'erreur *
 *                                                 *
 * sortie : un etat d'erreur                       *
 ***************************************************/

int errdirecte(int cod, char *arg) {

		fprintf(stderr, "\n%s%s%s.", arg && *arg? arg : "", arg? " : ": "", msgMARS[cod].msg);
	return 0;
}

/****************************************
 * Affiche le resultat de la traduction *
 *                                      *
 * entree : nom du programme            *
 *          nombre d'instructions       *
 *          premiere instruction        *
 *                                      *
 * sortie : neant                       *
 ****************************************/

void result(char *nomfic, int nbinstr, int entree, int numcamp) {

register int numfen = numcamp + FENTITR1 -1;

	if (modsortie == GRAPH || modsortie == MIXTE) {

		/* les trois appels sont nessecaires car les entetes
			de ces messages sont egalement des formats */
		affmsg( (char *) msgMARS[TRADOK1].msg, nomfic, numfen);
		affint( (char *) msgMARS[TRADOK2].msg, nbinstr, numfen);
		affint( (char *) msgMARS[TRADOK3].msg, entree, numfen);
	}
	else {
		fprintf(stdout, (char *) msgMARS[TRADOK1].msg, nomfic);
		fprintf(stdout, (char *) msgMARS[TRADOK2].msg, nbinstr);
		fprintf(stdout, (char *) msgMARS[TRADOK3].msg, entree);
	}

	/* redirection du resultat vers le fichier de sortie */
	if (sortie) {
		fprintf(sortie, (char *) msgMARS[TRADOK1].msg, nomfic);
		fprintf(sortie, (char *) msgMARS[TRADOK2].msg, nbinstr);
		fprintf(sortie, (char *) msgMARS[TRADOK3].msg, entree);
	}
}

int msgtrad(int codmsg, char *arg, int numcamp) {

	if (modsortie == GRAPH || modsortie == MIXTE)
		affmsg( (char *) msgMARS[codmsg].msg, arg, numcamp);
	else 
		fprintf(stdout, (char *) msgMARS[codmsg].msg, arg);

	return 0;

}


/*************************************
 * Partie graphique de la            *
 * messagerie.                       *
 *                                   *
 * entree :                          *
 *                                   *
 * sortie : neant                    *
 *************************************/

void msginit(void) {

	if (modsortie != GRAPH && modsortie != MIXTE) return;
	visuinit();
}

void msgpoint(int numcase) {

	if (modsortie != GRAPH && modsortie != MIXTE) return;
	affpoint(numcase);
}

void msgnoyau(void) {

	if (modsortie != GRAPH && modsortie != MIXTE ) return;
	affnoyau();
}

void msgfin(void) {

	if (modsortie != GRAPH && modsortie != MIXTE ) return;
	visuexit();
}


/*************************************
 * Messages de MARS en cours         *
 * d'execution.                      *
 * indiquer l'instruction qui vient  *
 * d'etre executee                   *
 *                                   *
 * entree : le code du message       *
 *                                   *
 * sortie : neant                    *
 *************************************/

void msginstr(int numcase, int numinstr,
	int adrA, int argA, int adrB, int argB, int numcamp){

	if (modsortie == TEXTE) {
	
		fprintf(stdout, "%4d : %s %c %-5d (%-4d) %c %-5d (%-4d)", 
			numcase, nommnemo(numinstr), 
			symboleadr(adrA)? symboleadr(adrA): ' ', argA, calcadr(numcase,0, NAFFECT),
			symboleadr(adrB)? symboleadr(adrB): ' ', argB, calcadr(numcase,1, NAFFECT));

		/* tabulation entre les listes d'execution de chaque camp */
		if (numcamp == 1)
			fprintf(stdout, "\t");

		if (nbprog == 1 || numcamp == 2)
			fprintf(stdout, "%s", "\n");
	}
	else 
		if (modsortie == MIXTE) {
			affint("%4d : ", numcase , numcamp);
			affmsg("%s ", nommnemo(numinstr), numcamp);
			affcar(symboleadr(adrA), numcamp);
			affint("%-5d ", argA, numcamp);
			affint("(%-5d) ", calcadr(numcase,0, NAFFECT), numcamp);
			affcar(symboleadr(adrB), numcamp);
			affint("%-5d ", argB, numcamp);
			affint("(%-5d)\n", calcadr(numcase,1, NAFFECT), numcamp);
		}

	/* redirection du resultat vers le fichier de sortie */
	if (sortie) {
		fprintf(sortie, "%4d : %s %c %-5d (%-4d) %c %-5d (%-4d)", 
			numcase, nommnemo(numinstr), 
			symboleadr(adrA)? symboleadr(adrA): ' ', argA, calcadr(numcase,0, NAFFECT),
			symboleadr(adrB)? symboleadr(adrB): ' ', argB, calcadr(numcase,1, NAFFECT));

		/* tabulation entre les listes d'execution de chaque camp */
		if (numcamp == 1)
			fprintf(sortie, "\t");

		if (nbprog == 1 || numcamp == 2)
			fprintf(sortie, "%s", "\n");
	}
}

void msgintro(int numcombat, int numcamp) {

register int i = 0;

	if (modsortie == GRAPH || modsortie == MIXTE) {
	
		affmsg("\n%s", (char *) msgMARS[NUMCOMBAT].msg, numcamp);
		affint("%d\n", numcombat, numcamp);
	}
	else
		fprintf(stdout, "\n%s%d\n", (char *) msgMARS[NUMCOMBAT].msg, numcombat);

	/* redirection du resultat vers le fichier de sortie */
	if (sortie) {
		if (numcamp == 2)
			for (i=0; i < 48; i++)
				fprintf(sortie, "%c",' ');
		else
			fprintf(sortie, "\n");

		fprintf(sortie, "\n%s%d\n", (char *) msgMARS[NUMCOMBAT].msg, numcombat);
	}
}


void msgexe(int codmsg, int numcase, int numcamp) {

register int i = 0;

	if (modsortie == MIXTE) { 
		affint("%4d : ", numcase , numcamp);
		affmsg("%s", (char *) msgMARS[codmsg].msg, numcamp);
		affcar('\n', numcamp);
	}
	else 
		if (modsortie == TEXTE) {
			if (numcamp == 2)
				for (i=0; i < 48; i++)
					fprintf(stdout, "%c",' ');
			else
				fprintf(stdout, "\n");
	
			fprintf(stdout, "%4d : %s%c", numcase, (char *) msgMARS[codmsg].msg, (numcamp == 2)? '\n': '\0');
		}

	/* redirection du resultat vers le fichier de sortie */
	if (sortie) {
		if (numcamp == 2)
			for (i=0; i < 48; i++)
				fprintf(sortie, "%c",' ');
		else
			fprintf(sortie, "\n");

		fprintf(sortie, "%4d : %s%c", numcase, (char *) msgMARS[codmsg].msg, (numcamp == 2)? '\n': '\0');
	}
}

void msgperte(int codmsg, char *nomprog, int numcamp) {

register int i = 0;

	if (modsortie == GRAPH || modsortie == MIXTE) { 
		affmsg("%s", nomprog, numcamp);
		affmsg("%s", (char *) msgMARS[codmsg].msg, numcamp);
		affcar('\n', numcamp);
	}
	else 
		if (modsortie == TEXTE) {
			if (numcamp == 2)
				for (i=0; i < 48; i++)
					fprintf(stdout, "%c",' ');
			else
				fprintf(stdout, "\n");
			fprintf(stdout, "%s%s%c", nomprog, (char *) msgMARS[codmsg].msg, (numcamp == 2)? '\n': '\0');
	
			if (codmsg == MORT)
				fprintf(stdout, "\n");
		}	

	/* redirection du resultat vers le fichier de sortie */
	if (sortie) {
		if (numcamp == 2)
			for (i=0; i < 48; i++)
				fprintf(sortie, "%c",' ');
		else
			fprintf(sortie, "\n");
		fprintf(sortie, "%s%s%c", nomprog, (char *) msgMARS[codmsg].msg, (numcamp == 2)? '\n': '\0');

		if (codmsg == MORT)
			fprintf(sortie, "\n");
	}
}

void msgstat(Mstat *stat, char *nomprog, int numcamp) {

	if (modsortie == GRAPH || modsortie == MIXTE) {
		
		affmsg((char *) msgMARS[TRADOK1].msg, nomprog, numcamp);	
		affint((char *) msgMARS[STATLOC].msg, stat->locinit, numcamp);	
		affint((char *) msgMARS[STATINSTR].msg, stat->nbinstr, numcamp);	
		affint((char *) msgMARS[STATPCT].msg, stat->nbpct, numcamp);	
		affint((char *) msgMARS[STATSPL].msg, stat->nbspl, numcamp);	
		affint((char *) msgMARS[STATNOP].msg, stat->nbnop, numcamp);	
		affint((char *) msgMARS[STATCOUP].msg, stat->nbcoup, numcamp);	
		affint((char *) msgMARS[STATPERTE].msg, stat->nbperte, numcamp);	
		affint((char *) msgMARS[STATMUTE].msg, stat->nbmute, numcamp);	
		
	}
	else {
		fprintf(stdout, (char *) msgMARS[TRADOK1].msg, nomprog, numcamp);	
		fprintf(stdout, (char *) msgMARS[STATLOC].msg, stat->locinit);	
		fprintf(stdout, (char *) msgMARS[STATINSTR].msg, stat->nbinstr);	
		fprintf(stdout, (char *) msgMARS[STATPCT].msg, stat->nbpct);	
		fprintf(stdout, (char *) msgMARS[STATSPL].msg, stat->nbspl);	
		fprintf(stdout, (char *) msgMARS[STATNOP].msg, stat->nbnop);	
		fprintf(stdout, (char *) msgMARS[STATCOUP].msg, stat->nbcoup);	
		fprintf(stdout, (char *) msgMARS[STATPERTE].msg, stat->nbperte);	
		fprintf(stdout, (char *) msgMARS[STATMUTE].msg, stat->nbmute);	
	}

	/* redirection du resultat vers le fichier de sortie */
	if (sortie) {
		fprintf(sortie, (char *) msgMARS[TRADOK1].msg, nomprog, numcamp);	
		fprintf(sortie, (char *) msgMARS[STATLOC].msg, stat->locinit);	
		fprintf(sortie, (char *) msgMARS[STATINSTR].msg, stat->nbinstr);	
		fprintf(sortie, (char *) msgMARS[STATPCT].msg, stat->nbpct);	
		fprintf(sortie, (char *) msgMARS[STATSPL].msg, stat->nbspl);	
		fprintf(sortie, (char *) msgMARS[STATNOP].msg, stat->nbnop);	
		fprintf(sortie, (char *) msgMARS[STATCOUP].msg, stat->nbcoup);	
		fprintf(sortie, (char *) msgMARS[STATPERTE].msg, stat->nbperte);	
		fprintf(sortie, (char *) msgMARS[STATMUTE].msg, stat->nbmute);	
	}
}

/*************************************
 * Mise a jour de la sortie standard *
 *                                   *
 * entree : nom du fichier           *
 *                                   *
 * sortie : neant                    *
 *************************************/

short maj_sortie(char *nomfic) {

	sortie = fopen(nomfic, "w");
	if (sortie) return 1;
	return 0;
}


void ferme_sortie(void) {

	if (sortie)
		fclose(sortie);

}

void maj_graph(char mode) {

	if (mode >= 0)
		modsortie = mode;
}

void maj_msg(int totalprog) {

	if (totalprog)
		nbprog = totalprog;
}

/*************************************
 * Indiquer le mode utilisation du   *
 * programme.                        *
 *                                   *
 * entree : le nom du programme      *
 *                                   *
 * sortie : 1                        *
 *************************************/

/* separateur de repertoire dans un nom absolu de fichier */
#define REPSEPAR '/'

int usage(char *nomprog) {

	/* supprimer la reference absolue au nom de fichier */
	if ( strrchr(nomprog, REPSEPAR)  )
		nomprog = strrchr(nomprog, REPSEPAR)+1;

	fprintf(stderr, "MARS : la guerre des programmes\n");
	fprintf(stderr, "Usage : %s [options] [nom_prog1] [nom_prog2]\n", nomprog);
	fprintf(stderr, "Options parmi  :\n");
	fprintf(stderr, "   -t[nombre]  : nombre de combats a effectuer.\n");
	fprintf(stderr, "   -c[nombre]  : nombre de cycles par combats.\n");
	fprintf(stderr, "   -p[nombre]  : position du second programme.\n");
	fprintf(stderr, "   -g[nombre]  : mode d'affichage (g0 = texte, g1 = graphique, g2 = mixte)\n");
	fprintf(stderr, "   -o[fichier] : nom du fichier de sortie.\n");
	fprintf(stderr, "   -e          : traiter l'evolution.\n");
	fprintf(stderr, "Si un seul nom est donne, il s'executera seul\n");

	return 1;
}

