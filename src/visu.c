/******************************************
 * Projet  : corewar                      *
 * Fichier : visu.c                       *
 * Visualisation des activites du noyau   *
 *                                        *
 * NB : le mode graphique PC qui a ete    *
 * choisi est le mode EGA 16 (16*640*350) *
 * On ne voit presque rien, mais c'est le *
 * mode le plus fin qu'accepte ma carte   *
 ******************************************/

#include "inc/core.h"
#include "inc/instruct.h"
#include "inc/visu.h"

/* model large */

/* symbole en 4x7 */

char symini[7][4] = {

	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,1,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
};

char symdat[7][4] = {

	{1,0,1,0},
	{1,0,1,0},
	{0,1,0,0},
	{0,1,0,0},
	{1,0,1,0},
	{1,0,1,0},
	{1,0,1,0}
};

char symmov[7][4] = {

	{0,0,0,0},
	{0,0,0,0},
	{0,1,1,0},
	{0,1,1,0},
	{0,1,1,0},
	{0,0,0,0},
	{0,0,0,0}
};

char symcmp[7][4]  = {

	{1,1,1,0},
	{1,0,1,0},
	{0,0,1,0},
	{0,1,1,0},
	{0,1,0,0},
	{0,0,0,0},
	{0,1,0,0}
};

char symadd[7][4]  = {

	{0,0,0,0},
	{0,1,0,0},
	{0,1,0,0},
	{1,1,1,0},
	{0,1,0,0},
	{0,1,0,0},
	{0,0,0,0}
};

char symsub[7][4]  = {

	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,1,1,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0}
};

char symjmp[7][4]  = {

	{0,0,1,0},
	{0,0,1,0},
	{0,1,1,0},
	{0,1,0,0},
	{1,1,0,0},
	{1,0,0,0},
	{1,0,0,0}
};

char symdec[7][4]  = {

	{1,0,0,0},
	{1,0,0,0},
	{1,1,0,0},
	{0,1,0,0},
	{0,1,1,0},
	{0,0,1,0},
	{0,0,1,0}
};

char symspl[7][4]  = {

	{1,0,0,0},
	{0,1,0,0},
	{0,0,1,0},
	{0,0,1,0},
	{0,0,1,0},
	{0,1,0,0},
	{1,0,0,0}
};

char sympct[7][4]  = {

	{1,1,1,0},
	{1,0,1,0},
	{1,0,1,0},
	{1,0,1,0},
	{1,0,1,0},
	{1,0,1,0},
	{1,1,1,0}
};

char symtst[7][4] = {

	{1,1,1,0},
	{0,0,0,0},
	{1,0,1,0},
	{0,0,0,0},
	{1,0,1,0},
	{0,0,0,0},
	{1,1,1,0}
};

/**************************************
 *
 * type police bitmap
 *
 **************************************/

typedef struct {

	char **mapadr;	/* table des adresses de matrices */
	int maxsym;	/* nombre de symboles pour cette police */
	int larg;	/* largeur d'une matrice, en car ou pixel */
	int haut;	/* hauteur d'une matrice, en car ou pixel */

} policeBM;


/*******************
 *
 * police core war
 *
 *******************/

char *bitmapcw[] = {

	&**symdat, &**symmov, &**symadd, &**symsub, &**symcmp,
	&**symjmp, &**symdec, &**symspl, &**sympct, &**symtst,
	&**symini
};

policeBM cwpolice = {bitmapcw, 11, 4, 7};


/***********************
 *
 * fonction asm pour EGA
 *
 ***********************/

extern void printEGA (int x, int y, int coulfond, int coultxt,
	int code, policeBM *custom, int page);

extern void setmode(int mode);
extern int getmode(void);

extern void actplan (int plan, int mode);
extern void actpage (int page);


#define EGABLANC2 15
#define EGABLANC   7
#define EGAMARRON  6
#define EGAVIOLET  5
#define EGAROUGE   4
#define EGACYAN    3
#define EGAVERT    2
#define EGABLEU    1
#define EGANOIR    0


/****************
 *
 * fonction visu
 *
 ****************/

#define MAXCOL 640
#define MAXLIG 350

typedef struct {

	int x;
	int y;

} point;


point calcoord (int numcase) {

static point coord = {0,0};

	coord.x = numcase%(MAXCOL/4);
	coord.y = ((numcase)/(MAXCOL/4))*7;
	return coord;

}

void visuinit(void) {

	setmode(16); /* mode EGA 16 couleurs 640*350 */
}


void affnoyau(void) {

register int x = 0;
register int y = 0;
register int numcase = 0;

	for (y=0; y<=MAXLIG; y+=7)
		for (x=0; x<=MAXCOL; x+=4)
			if ( numcase++ < MAXCASE )
		printEGA ( x, y, EGABLANC, EGANOIR, 10, &cwpolice, 0);

}


void affpoint (int numcase) {

auto point p = calcoord(numcase);
register coultxt = 0;
register coulfnd = 0;

	switch ( appart(numcase) ) {

	case 1:
		coultxt = EGAVERT;
		coulfnd = EGANOIR;
		break;

	case 2:
		coultxt = EGAROUGE;
		coulfnd = EGANOIR;
		break;

	default:
		coultxt = EGABLANC;
		coulfnd = EGANOIR;
		break;
	}

	printEGA ( p.x, p.y, coulfnd, coultxt, codeoper(numcase), &cwpolice, 0);

}


void affmsg(char *format, char *msg, int fen) {

}

void affcar(char car, int fen)  {


}

void affint(char *format, int val, int fen) {

}

void visuexit(void) {

}
