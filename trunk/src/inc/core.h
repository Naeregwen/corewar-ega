/******************************************
 * Projet  : corewar                      *
 * Fichier : core.h                       *
 * Structure d'anneau de memoire          *
 * Auteur  : NVA                          *
 * Date    : 27/11/94                     *
 ******************************************/

#ifndef _CORE_H_
#define _CORE_H_

#include "redcodop.h"

#define MAXCASE 8000

/* calcul dune adresse valide */
/* modulo MAXCASE pour le type entier */
/* ajout de MAXCASE pour les negatifs */
/* modulo MAXCASE pour les positifs */

#define VALIDE(add) ((MAXCASE+(add)%MAXCASE)%MAXCASE)


short noyaucreer (void);
void noyaudetruire (void);

char codeoper (int numcase);
char appart (int numcase);
char protect (int numcase);
int modeadr (int numcase, int numarg);
int valarg (int numcase, int numarg);

enum {NAFFECT, AFFECT};
int calcadr (signed numcase, short numarg, char mode);

void proteger(int numcase, int niv);
void deproteger(int numcase);
int copieroper (operation *oper, int loc);
int copiercase (int source, int dest, int locinit);
int copiervaleur (int val, int loc, int locinit);

int incDAT (int loc, int locinit);
int decDAT (int loc, int locinit);
int addDAT (int val, int loc, int locinit);
int subDAT (int val, int loc, int locinit);
int addADR (int locA, int locB, int locinit);
int subADR (int locA, int locB, int locinit);

#endif
