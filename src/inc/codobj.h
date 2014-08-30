/****************************
 * Projet  : corewar        *
 * Fichier : codobj.h       *
 * Traitement du code objet *
 * Auteur  : NVA            *
 * Date    : 20/11/94       *
 ****************************/

#ifndef _CODOBJ_H_
#define _CODOBJ_H_

/* resultat de la traduction numerique */

#include "redcodop.h"

typedef struct cellobj {

	operation instr;
	char *argires[MAXARG];
	/* liens sur les etiquettes de dicoires,
	 * quand l'instruction n'est pas completement resolue
	 * ce ne sont pas les liens initiaux sur ces chaines
	 * Ce derniers sont ceux de dicoires lui-meme */
	struct cellobj *suiv;

} codobj;

codobj *codobjcreer (char codop);
codobj *codobjchainer (codobj *elem, codobj *liste);
codobj *codobjtete (codobj *elem, codobj *liste);
codobj *codobjeffacer (codobj *elem);
codobj *codobjdetruire (codobj *liste);

/* la premiere "fausse" cellule permet d'eviter les cas de code de
 * longueur nul.
 * Elle sera changee en END puis recevra des infos sur le programme
 * qu'elle precede  :
 * - nombre d'instructions (DAT compris, pas END)
 * - premiere instruction a executer (point d'entree) */

/* tete de code */
#define TETEPROG(prog) ((prog)->instr.codop)
/* appartenance */
#define APPART(prog) ((prog)->instr.appart)
/* nombre d'instructions */
#define NBINSTR(prog) ((prog)->instr.tabarg[0].val)
/* premiere instruction executable */
#define INSTR1(prog) ((prog)->instr.tabarg[1].val)


#endif
