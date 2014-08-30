
/* model large */

/* symbole en 4x7 */

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
	&**symjmp, &**symdec, &**symspl, &**sympct, &**symtst
};

policeBM cwpolice = {bitmapcw, 10, 4, 7};

/**********
 *
 * mode EGA
 *
 **********/

/********************
 *
 * type ecran virtuel
 *
 ********************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define EGA13 13
#define EGA14 14
#define EGA15 15

#define EGAPLAN	4
#define EGAMODE	4
#define EGACOUL 16

#define EGABLANC2 15
#define EGABLANC   7
#define EGAMARRON  6
#define EGAVIOLET  5
#define EGAROUGE   4
#define EGACYAN    3
#define EGAVERT    2
#define EGABLEU    1
#define EGANOIR    0

#define EGA13bps 8000
#define EGA14bps 16000
#define EGA16bps 28000

#define EGABASE 0xA000

#define MODE(m) ((m - EGA13) % EGAMODE)

const struct {

	int bpp; /* octets par page */
	int bpl; /* octets par ligne */
	int ppb; /* pixels par octet sur un plan */


} EGAmode[EGAMODE] = {

	{ EGA13bps, 40, 8},
	{ EGA14bps, 80, 8},
	{ 0,        80, 8},
	{ EGA16bps, 80, 8}
};

enum { LECTURE, FINLECTURE, ECRITURE, FINECRITURE };


/********************
 *
 * type ecran virtuel
 *
 ********************/

typedef char **VScr; /* 4 pour le mode EGA */


/**************
 *
 * fonction asm
 *
 **************/

extern void printEGA (int x, int y, int coulfond, int coultxt,
	int code, policeBM *custom, int page);

extern void setmode(int mode);
extern int getmode(void);

extern void actplan (int plan, int mode);
extern void actpage (int page);


/********************
 *
 * type ecran virtuel
 *
 ********************/

VScr *VScrAlloc (void) {

/* suppose qu'on est en EGA,
   sinon risque de crash */

VScr *nouv = (VScr*) calloc (1, EGAPLAN * sizeof(char *));
int pgsz = sizeof(char) * EGAmode[ (getmode() - EGA13) % EGAMODE ].bpp;
int plan = 0;

/*	if ( mode == 15 || mode < 13 || mode > 16 ) return;
	mode = (mode - EGA13) % EGAMODE; */

	if (!nouv) return 0;

	/* attribution des valeurs au pointeur de plans */
	/* decouper le bloc de memoire en autant de plan */
	for (plan = 0; plan < EGAPLAN; plan++)
		if ( !( *nouv[plan] = (char *) calloc (1, pgsz)) )
			return NULL;

	return nouv;
}

void BlocVersEGA (VScr *myscr, int X1, int X2, int Y1, int Y2) {

/* suppose qu'on est en EGA,
   sinon risque de crash */

register int mode = (getmode() - EGA13) % EGAMODE;
register int plan = 0;
register int ligne = 0;

register int bpl = (X2 - X1 + 1) / EGAmode[mode].bpl + 1;


	/* pour chaque plan */
	for (plan = 0; plan < EGAPLAN; plan++)

		actplan (plan, ECRITURE);

		/* recopie du tampon vers la RAM video */
		for (ligne = Y1 - 1; ligne < Y2; ligne++) {
			movedata
			(  FP_SEG (*myscr[plan]), FP_OFF (*myscr[plan]),
				0xA000, 0,
				EGAmode[mode].bpp
			);

	}

	ligne = bpl;
	actplan (0, FINECRITURE);

}


/*****************
 *
 * type bitmap EGA
 *
 *****************/

typedef struct {

	int haut; 	/* en pixel */
	int larg; 	/* en pixel */
	char **bitmap; 	/* table des pixels/plan (voire plus) */

} bmEGA;

bmEGA *bmEGAAlloc (int larg, int haut) {

bmEGA *nouv = (bmEGA*) calloc (1, sizeof(bmEGA));
int pgsz = (larg / EGAmode[MODE(getmode())].ppb + 1) * haut ;
int plan = 0;

	if (!nouv || !pgsz) return NULL;
	if ( !(nouv->bitmap = (char **) calloc (EGAPLAN, sizeof(char *)))
			|| !(*nouv->bitmap = (char *) calloc (EGAPLAN, pgsz)) )
		return NULL;

	/* distribution des morceaux de memoire a chaque pointeur de plan */
	/* sauf au premier */
	for (plan = 1; plan < EGAPLAN; plan++)
		nouv->bitmap[plan] = *nouv->bitmap + plan * pgsz;

	nouv->haut = haut;
	nouv->larg = larg;

	return nouv;
}

void bmVersEGA (bmEGA *img, int x, int y, int page) {

/* suppose qu'on est en EGA,
   sinon risque de crash */

auto int mode = MODE(getmode());
register unsigned bpl = img->larg / EGAmode[mode].ppb + 1;
register int plan = 0;
register int ligne = 0;
auto unsigned segpag = EGABASE + (page % EGAPLAN) * EGAmode[mode].bpp;

	/* pour chaque plan */
	for (plan = 0; plan < EGAPLAN; plan++) {

		actplan (plan, ECRITURE);

		/* recopie du tampon vers la RAM video */
		for (ligne = y; ligne < img->haut + y ; ligne++) {

			movedata
			(
				FP_SEG (img->bitmap[plan] + (ligne - y) * bpl),
				FP_OFF (img->bitmap[plan] + (ligne - y) * bpl),
				segpag,
				(ligne * EGAmode[mode].bpl) + x / EGAmode[mode].ppb,
				bpl
			);
		}
	}

	actplan (0, FINECRITURE);

}


void EGAVersbm (bmEGA *img, int x, int y, int page) {

/* suppose qu'on est en EGA,
   sinon risque de crash */

auto int mode = MODE(getmode());
register unsigned bpl = img->larg / EGAmode[mode].ppb + 1;
register int plan = 0;
register int ligne = 0;
auto unsigned segpag = EGABASE + (page % EGAPLAN) * EGAmode[mode].bpp;

	/* pour chaque plan */
	for (plan = 0; plan < EGAPLAN; plan++) {

		actplan (plan, LECTURE);

		/* recopie du tampon vers la RAM video */
		for (ligne = y; ligne < img->haut + y ; ligne++) {

			movedata
			(
				segpag,
				(ligne * EGAmode[mode].bpl) + x / EGAmode[mode].ppb,
				FP_SEG (img->bitmap[plan] + (ligne - y) * bpl),
				FP_OFF (img->bitmap[plan] + (ligne - y) * bpl),
				bpl
			);
		}
	}

	actplan (0, FINLECTURE);

}


void plotVEGA (int x, int y, int coul, bmEGA *img) {

register unsigned char masq = 7 - x % 8;
register int mode = MODE(getmode());
register int bpl = img->larg / EGAmode[mode].ppb + 1;
register int plan = 0;

	for (plan = 0; plan < EGAPLAN; plan++, coul >>= 1)
		*(img->bitmap[plan] + (y * bpl) + x / EGAmode[mode].ppb) =
		*(img->bitmap[plan] + (y * bpl) + x / EGAmode[mode].ppb)
		& (~(1 << masq) | ( (coul & 1) << masq));



}

 
void printVEGA (int x, int y, int coulfond, int coultxt,
	int code, policeBM *police, bmEGA *img) {

register int x1 = 0;
register int y1 = 0;

	for (y1 = 0; y1 < police->haut && y+y1 < img->haut; y1++)
		for (x1 = 0; x1 < police->larg && x+x1 < img->larg; x1++)
			plotVEGA(x+x1, y+y1, *(police->mapadr[code%(police->maxsym)]
				+ y1 * police->larg + x1 )? coultxt: coulfond, img);


}


void printLEGA (void) {
/*
auto unsigned segtbl = 0;
auto unsigned offtbl = 0;
auto largcar = 0;
auto totallig = 0;
auto nbpoint = 0;
*/
	/* lecture du mode courant et de la largeur de car */
	/* lecture de l'adresse de la police systeme courante */
	__asm__  (
		"push ax"
		"push bx"
		"push cx"
		"push dx"
		"push es"
		"push bp"

		"mov ah, 0x0F"
		"int 0x10"

		"xchg al, ah"
		"sub ah, ah"
		"sub ch, ch"
		"sub dh, dh"
		"mov largcar, ax"
		"mov nbpoint, cx"
		"inc dx"
		"mov totalig, dx"

		"mov ah, 0x011"
		"mov al, 0x030"
		"mov bh, 1"
		"int 0x10"

		"xchg ax, bp"
		"pop bp"
		"mov segtbl, es"
		"mov offtbl, ax"

		"pop es"
		"pop cx"
		"pop bx"
		"pop bx"
		"pop ax"
	);


	/* imprimer un 'A' (beurk) */
	/* calculer l'offset de la bitmap */
	/* offset = adresse base + code(A) * taillebm(haut*larg) */
	/* taillebm = largcar * nblig */


}




void invert (bmEGA *img) {

register int x;
register int pgsz = (img->larg / EGAmode[MODE(getmode())].ppb + 1) * img->haut;

	for (x=0; x < EGAPLAN * pgsz; x++)
		*(*img->bitmap + x) = ~(*(*img->bitmap + x));

}

/******
 *
 * main
 *
 ******/

int main (void) {

extern void kbdwait(void);

int x,y,sym;
bmEGA *bloc = NULL;

	setmode(13);

	bloc = bmEGAAlloc(20,20);

	for (y=0; y < 200; y+=7)
		for (x=sym=0; x < 320; x+=4, sym++)
			printEGA (x,y,EGAROUGE,EGAVERT,sym, &cwpolice,0);

	EGAVersbm (bloc, 20,20,0);

	for (y=0; y < 20; y+=7)
		for (x=sym=0; x < 20; x+=4, sym++) {
			printVEGA(x,y,EGABLANC,EGAVIOLET,2, &cwpolice, bloc);
			bmVersEGA (bloc, 20, 20, 0);
		}

	printLEGA();
	printf ("coucou");
/*	invert(bloc);*/

	kbdwait();

}
