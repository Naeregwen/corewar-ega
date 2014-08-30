/****************************
 * Projet  : corewar        *
 * Fichier : redcodop.h     *
 * type operation redcode   *
 * Auteur  : NVA            *
 * Date    : 25/11/94       *
 ****************************/

#ifndef _REDCODOP_H_
#define _REDCODOP_H_

/* #include "instruct.h" nombre d'arguments */

#define MAXARG 2


typedef struct {

	char adr; /* valeur du mode d'adressage */
	signed val; /* valeur de l'adresse ou de la donnee */

} argument;

typedef struct {

	char codop; /* indice de l'operation */
	char appart; /* numero du camp d'apartenance (0 personne, 1 camp1, ...) */
	char protect; /* 0 : la case n'est pas protegee, 1 : elle l'est */
	argument tabarg[MAXARG]; /* table d'arguments */

} operation;

#endif
