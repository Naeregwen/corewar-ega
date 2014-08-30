/*****************************************
 * Projet  : corewar                     *
 * Fichier : dicoetiq.h                  *
 * Dictionnaire d'etiquettes             *
 * Auteur  : NVA                         *
 * Date    : 20/11/94                    *
 *****************************************/

#ifndef _DICOETIQ_H_
#define _DICOETIQ_H_

short dicoetiqcreer(void);
void dicoeitqeffacer(void);
void dicoetiqdetruire(void);
int etiqexist(char *nom);
short etiqajouter(char *nom, signed numl);
short etiqenlever(char *nom);
long nbetiq(void);

#endif
