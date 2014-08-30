#ifndef TRAITARG_H_
#define TRAITARG_H_

#endif /*TRAITARG_H_*/

#define OPTPOS    'P' /* caractere pour modifier l'option de position */
#define OPTTRY    'T' /* caractere pour modifier l'option de test */
#define OPTCYCLIM 'C' /* caractere pour modifier l'option de limite de cycle */
#define OPTEVOL   'E' /* caractere pour modifier l'option d'evolution */
#define OPTGRAPH  'G' /* caractere pour modifier l'option graphique */
#define OPTHALTE  'D' /* caractere pour modifier l'option de halte */
#define OPTSORTIE 'O' /* caractere pour modifier l'option de sortie */

#define MAXCYCLE 10000
#define MAXNOMFIC 256


typedef struct {

	char *nomprog1; /* nom du premier programme */
	char *nomprog2; /* nom du second programme */
	char *sortie; /* fichier de sortie */
	int pos2; /* position du second programme */
	int cyclelim; /* nombre de cycle par combats */
	char evol; /* 0 = evolution ignoree, 1 = evolution traite */
	char graph; /* 0 = mode texte, 1 = mode graphique */
	int test; /* nombre de combats a affectuer */

} arglig;


arglig *argligcreer(void);
void argligdetruire(arglig *elem);
short traitarg(int argc, char **argv, arglig *args);
