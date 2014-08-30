/****************************
 * Projet  : corewar        *
 * Fichier : instruct.h     *
 * Jeu d'instructions       *
 * Auteur  : NVA            *
 * Date    : 25/11/94       *
 ****************************/

#ifndef _INSTRUCT_H_
#define _INSTRUCT_H_

#define MAXARG 2

/* liste des caracteres codant un mode d'adressage */
/* les codes correspondant sont 1, 2, 3 et 4. Le zero est pour le relatif */

#define MAXCODADR 5

enum { RELATIF, IMMEDIAT, INDIRECT, PREDEC, PREINC };

#define MAXLGMNEMO 3

/* table des instructions et de leur code d'adressage autorises */

#define MAXMNEMO 14
#define MAXDIRECTIVE 1

enum { DAT, MOV, ADD, SUB, JMP, JMZ, JMG, DJZ, CMP, SPL, DJN, JMN, PCT, TST, END };

/* fonctions */

short estmnemo (char *mot);
short estdirective (char *mot);
short estinstruction (char *mot);
short estcodadr (char c);
short estmodadr(int codadr, int numarg, char codop);
short maxarg(char codop);
short minarg(char codop);
char *nommnemo(char codop);
char symboleadr(char modadr);

#endif
