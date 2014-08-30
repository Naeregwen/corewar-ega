/************************/
/* fonctions de hachage */
/************************/

/*********************************/
/* hachage par somme bit par bit */
/*********************************/

unsigned hbit(char *s, unsigned max) {

register const char *c=s;   /* pointeur de caractere dans s */
register unsigned i=0; /* indice de bit dans *c */
register unsigned som=0;  /* resultat du hachage */

	for ( c=s; *c; c++ )
		for ( i=sizeof(char)*8-1; i; i--)
			if ( *c & (1<<i) )
				som=(som+1)%max;

	return som;

}


/*****************************/
/* hachage par somme logique */
/*****************************/

unsigned hlog(char *s, unsigned max) {

/* formule : som(n+1) = som(n) (+) car(n+1) */

register const char *c=s;  /* pointeur de caractere dans s */
register unsigned som=0;  /* resultat du hachage */

	while (*c) som^=*c++;

	return som%max;

}


/*****************************************/
/* hachage par somme logique et decalage */
/*****************************************/

unsigned hdlog(char *s, unsigned max) {

/* formule : som(n+1) = 2*som(n) (+) car(n+1) */

register const char *c=s;  /* pointeur de caractere dans s */
register unsigned som=0;  /* resultat du hachage */

	while (*c) som = (som<<1) ^ (*c++);

	return som%max;

}


/******************/
/* hachage triple */
/******************/

unsigned htripl(char *s, unsigned max) {

	return (hbit(s,max)*hlog(s,max)*hdlog(s,max))%max;

}
