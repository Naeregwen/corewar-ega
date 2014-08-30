/*******************************************
 * Projet  : corewar                       *
 * Fichier : msgerr.h                      *
 * Gestion des erreurs d'execution de mars *
 *******************************************/

#ifndef _MSGERR_H_
#define _MSGERR_H_

#include "stat.h"

enum {
	OK,

	TRADCOMMENCE,
	TRADOK1,
	TRADOK2,
	TRADOK3,
	TRADIMPOSSIBLE,
	CHARGEMENT,

	ETIQNONALPHA, ETIQNONALNUM, ETIQLONG, ETIQEXIST, ETIQILLEGALE,
	MNEMONONALPHA, MNEMOLONG, INSTRINCONNU,
	MODADRILLEGAL,
	ARGMANQUANT, ARGINCORRECT, ARGILLEGAL,
	ARGSUPERFLU, ARGLONG, ARGNUMINCORRECT, 
	VIRGATTENDUE,
	CARSUPERFLU,
	SECONDILLEGAL,
	FININCONNUE,
	ARGINCONNU,

	LIMITEPROG,
	PASDEFIN,
	SEULEMENTDESDAT,
	FICHIERVIDE,
	FICHIERILLISIBLE,
	CODETROPLONG,
	TROPDECODE,

	NUMCOMBAT,
	CASEPROTEGEE,
	DATSEULEMENT,
	EVOLUTION,
	NONEXE,
	MORTE,
	PERTE,
	MORT,
	NUL,

	OPTINCONNUE,
	OPTVIDE,
	TROPDENOM,
	MAUVAISEPOS,
	MAUVAISELIM,
	MAUVAISFIC,

	STATLOC,
	STATINSTR,
	STATPCT,
	STATSPL,
	STATNOP,
	STATCOUP,
	STATPERTE,
	STATMUTE,

	MEMNONDISPO,
	MSGBREAK,
	MAXMSG
};

int usage(char *nomprog);

int enerrtrad(int cod, int lig, char *car, char *arg);
int errtrad(char *ligne, int numcamp);
int errdirecte(int coderr, char *arg);
int msgtrad(int codmsg, char *arg, int numcamp);
void result(char *nomfic, int nbinstr, int entree, int numcamp);

short maj_sortie(char *nomfic);
void ferme_sortie(void);

enum { TEXTE, GRAPH, MIXTE };
void maj_graph(char mode);
void maj_msg(int totalprog);

void msginit(void);
void msgpoint(int numcase);
void msgnoyau(void);
void msgstat(Mstat *stat, char *nomprog, int numcamp);
void msgfin(void);

void msgintro(int numcombat, int numcamp);
void msginstr(int numcase, int numinstr, int adrA, int argA, int adrB, int argB, int numcamp);
void msgexe(int codmsg, int numcase, int numcamp);
void msgperte(int codmsg, char *nomprog, int numcamp);

#endif


