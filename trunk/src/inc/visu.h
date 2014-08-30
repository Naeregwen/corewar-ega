/******************************************
 * Projet  : corewar                      *
 * Fichier : visu.h                       *
 * Visualisation des activites du noyau   *
 ******************************************/

#ifndef _VISU_H_
#define _VISU_H_

void visuinit(void);
void affnoyau(void);
void affpoint (int numcase);
void affmsg(char *format, char *msg, int fen);
void affcar(char car, int fen);
void affint(char *format, int val, int fen);
void visuexit(void);

#define MAXFEN 6
enum { FENTOTAL , FENCAMP1, FENCAMP2, FENTITR1, FENTITR2, FENNOYAU };

#endif
