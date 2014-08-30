#
# travail avec les modes EGA
#
#
#    .286    

# constantes EGA
.code16

.equ    EGA13,  13
.equ    EGA14,  14
.equ    EGA16,  16  # mode EGA 640x350x16

.equ    EGASEQ, 0x03C4  # sequencer EGA
.equ    EGAFIXCOL,  2
.equ    EGAGRAPH,   0x03CE  # port graphique
.equ    EGAFIXPIX,  8

.equ    EGAPLAN,    4   # nombre de plans par pixel

# calcul de l'adresse d'un pixel

.equ    EGAbase,    0x0A000 # adresse de base des mode EGA

.equ    EGA13bpp,   0x0200  # mode EGA 320x200x16, octets par page
.equ    EGA14bpp,   0x0400  # mode EGA 640x200x16, octets par page
.equ    EGA16bpp,   0x0800  # mode EGA 640x350x16, octets par page

.equ    EGA13bpl,   40  # mode EGA 320x200x16, octets par ligne
.equ    EGA14bpl,   80  # mode EGA 640x200x16, octets par ligne
.equ    EGA16bpl,   80  # mode EGA 640x350x16, octets par ligne

.equ    EGA13maxx,  320 # mode EGA 320x200x16, maximum horizontal
.equ    EGA14maxx,  640 # mode EGA 640x200x16, maximum horizontal
.equ    EGA16maxx,  640 # mode EGA 640x350x16, maximum horizontal

.equ    EGA13maxy,  200 # mode EGA 320x200x16, maximum vertical
.equ    EGA14maxy,  200 # mode EGA 640x200x16, maximum vertical
.equ    EGA16maxy,  350 # mode EGA 640x350x16, maximum vertical


#DGROUP: GROUP   $_TEXT

#;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
#
# CODE EGA
#
#;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#_TEXT:  SEGMENT $byte public 'CODE'

#    ASSUME  $ds:DGROUP,$CS:_TEXT

#;;;;;;;;;;;;;;;;;;;;;
#
# ecran physique
#
#;;;;;;;;;;;;;;;;;;;;;

#
# but :
# modes EGA 13, 14, 16 (16 couleurs, 4 plans)
# Donne … un pixel designe par ses coordonnees (x,y) et sa page,
# la couleur donnee
#
# mode par couleur
# un pixel (son code couleur EGA) est envoye au materiel via les ports
#
# entree :
#
# AX : x
# BX : y 
# DL : page
# DH : couleur
#
# sortie : neant
#
.globl $plotEGA13
plotEGA13:

# verifier les limites d'ecran

    cmpw    $EGA13maxx,%ax  # horizontal
    jge     EGAret

    cmpw    $EGA13maxy,%bx  # vertical
    jge     EGAret

# sauver le contexte

    pushw   %ax # AL contiendra le masque du pixel
    pushw   %cx # accumulateur
    pushw   %dx # pour l'adressage des ports
    pushw   %es # pour pointer l'octet
    pushw   %di # du pixel concerne

# calcul de l'adresse de base selon la page
# ES

    pushw   %ax
    pushw   %dx
    subb    %dh,%dh
    andb    $7,%dl  # page modulo 8
    movw    $EGA13bpp,%ax   # octets par page
    mulw    %dx # 200h*page
    addw    $EGAbase,%ax    # avec la base
    pushw   %ax
    popw    %es # en extra
    popw    %dx
    popw    %ax

# calcul de l'offset (x,y) du pixel : mode 13
# y*40 + x div 8 pour mode 13
# DI

    movw    %bx,%di # di = y
    movw    %bx,%cx # cx = y
    shlw    $3,%di  # y*8
    shlw    $5,%cx  # y*32

    jmp     addbas   # passer au code commun des fonctions

.globl $plotEGA14
plotEGA14:

# verifier les limites d'ecran

    cmpw    $EGA14maxx,%ax  # horizontal
    jge     EGAret

    cmpw    $EGA14maxy,%bx  # vertical
    jge     EGAret

# sauver le contexte

    pushw   %ax # AL contiendra le masque du pixel
    pushw   %cx # accumulateur
    pushw   %dx # pour l'adressage des ports
    pushw   %es # pour pointer l'octet
    pushw   %di # du pixel concerne

# calcul de l'adresse de base selon la page
# ES

    pushw   %ax
    pushw   %dx
    subb    %dh,%dh
    andb    $3,%dl  # page modulo 4
    movw    $EGA14bpp,%ax   # octets par page
    mulw    %dx # 400h*page
    addw    $EGAbase,%ax    # avec la base
    pushw   %ax
    popw    %es # en extra
    popw    %dx
    popw    %ax


    jmp     calcbas  # code commun au mode 14 et 16

EGAret: 
    jmp     plotEGAret

.globl $plotEGA16
plotEGA16:

# verifier les limites d'ecran

    cmpw    $EGA16maxx,%ax  # horizontal
    jge     EGAret

    cmpw    $EGA16maxy,%bx  # vertical
    jge     EGAret

# sauver le contexte

    pushw   %ax # AL contiendra le masque du pixel
    pushw   %cx # accumulateur
    pushw   %dx # pour l'adressage des ports
    pushw   %es # pour pointer l'octet
    pushw   %di # du pixel concerne

# calcul de l'adresse de base selon la page
# ES

    pushw   %ax
    subw    %ax,%ax
    andb    $1,%dl  # page modulo 2
    jz      calcbas16  # 400h*page
    addw    $EGA16bpp,%ax   # octets par page
calcbas16:  
    addw    $EGAbase,%ax    # avec la base
    pushw   %ax
    popw    %es
    popw    %ax

# calcul de l'offset (x,y) du pixel : mode 14,16
# y*80 + x div 8 pour mode 14,16 
# DI

calcbas:    
    movw    %bx,%di # di = y
    movw    %bx,%cx # cx = y
    shlw    $4,%di  # y*16
    shlw    $6,%cx  # y*64

# code commun des routines plotEGA

addbas: 
    addw    %cx,%di # y*(16+64) = y*80 ou y*(8+32) = y*40

    movw    %ax,%cx # cx = x
    shrw    $3,%cx  # x div 8

    addw    %cx,%di # y*octet par ligne + x div 8

# masque du pixel

    andb    $7,%al  # x mod 8
    movb    $7,%cl  # cl = 7
    subb    %al,%cl # cl = 7 - x mod 8
    movb    $1,%al  # masque :
    shlb    %cl,%al # 1 pour le pixel, 0 pour les voisins

    movb    %dh,%cl # sauver la couleur du pixel
    movb    %al,%ch # sauver le masque du pixel

# mise a 0 du pixel

    movb    $EGAFIXPIX,%al  # fonction FIXPIX
    movw    $EGAGRAPH,%dx
    outb    %al,%dx

    movb    %ch,%al # masque du pixel
    incw    %dx
    outb    %al,%dx

    movb    %es:(%di),%ah
    movb    $0, %es:(%di)    # 0

# mise en couleur du pixel

    movb    $EGAFIXCOL,%al  # colore un pixel
    movw    $EGASEQ,%dx
    outb    %al,%dx

    movb    %cl,%al # couleur du pixel
    incw    %dx
    outb    %al,%dx

    movb    %es:(%di),%ah
    movb    $0x0FF, %es:(%di)

# remise en l'etat des ports

    movb    $EGAFIXCOL,%al
    movw    $EGASEQ,%dx
    outb    %al,%dx

    movb    $15,%al
    incw    %dx
    outb    %al,%dx

    movb    $EGAFIXPIX,%al
    movw    $EGAGRAPH,%dx
    outb    %al,%dx

    movb    $0x0FF,%al
    incw    %dx
    outb    %al,%dx

# restaurer le contexte

    popw    %di
    pop     %es

    popw    %dx
    popw    %cx
    popw    %ax

plotEGAret:     
    ret 


#
# but    : afficher un bloc de pixel depuis une structure bitmap EGA
# vers l'ecran physique
#
# mode par plan
# chaque plan physique est ouvert en ecriture
# puis on y recopie le plan logique.
#
# entree :
#
# bmEGA *bitmap : ptr far
# int X1, int X2, int Y1, int Y2 : en pixel
#
# sortie : neant
#
#
/*
bitmap = %ss:+14(%bp)
.equ    X1, %ss:12(%bp)
.equ    X2, %ss:(%bp+10)
.equ    Y1, %ss:(%bp+8)
.equ    Y2, %ss:(%bp+6)
*/
.globl  $_plotbmEGA
_plotbmEGA:

    popw    %bp
    movw    %sp,%bp

# sauver le contexte

    push    %es
    push    %ds
    pushw   %si
    pushw   %di
    pushw   %bx
    pushw   %cx

# nombre d'octet par ligne = (X2 - X1 + 1 ) / 8 + 1

    movw    %ss:10(%bp),%bx

    cmpw    %ss:12(%bp),%bx # si inverses
    jge calcbpl
    xchgw   %ss:12(%bp),%bx
    movw    %bx,%ss:10(%bp)

calcbpl:    
    subw    12(%bp),%bx
    incw    %bx
    shlw    $1,%bx
    shlw    $1,%bx
    shlw    $1,%bx
    incw    %bx

# adresse du plan logique

    ldsw    %ss:14(%bp),%di # adresse de la table des plans
    lesw    (%di),%di   # adresse du plan dans la table

# adresse du plan physique (page 0)


# restaurer le contexte

    popw    %cx
    popw    %bx
    popw    %di
    popw    %si
    pop     %ds
    pop     %es

    pushw   %bp
    ret 


#;;;;;;;;;;;;;;;;;;;;;
#
# ecran virtuel
#
#;;;;;;;;;;;;;;;;;;;;;


# Donne … un pixel designe par ses coordonnees (x,y) la couleur donnee
# dans l'ecran virtuel
#
# entree :
#
# AX : x
# BX : y
# DH : couleur
# ES:DI : structure de plans
#
# sortie : neant
#
.globl $plotVEGA13
plotVEGA13:

# verifier les limites d'ecran

    cmpw    $EGA13maxx,%ax  # horizontal
    jge     EGAVret

    cmpw    $EGA13maxy,%bx  # vertical
    jge     EGAVret

# sauver le contexte

    pushw   %ax # AL contiendra le masque du pixel
    pushw   %bx # y*bpl + x div 8
    pushw   %cx # compteur
    pushw   %dx
    push    %es # pour pointer le bit
    pushw   %si # du pixel concerne dans chaque plan
    pushw   %di

# calcul de l'offset (x,y) du pixel : mode 13
# y*40 + x div 8 pour mode 13
# BX

    movw    %bx,%cx # cx = y
    shlw    $3,%bx  # y*8
    shlw    $5,%cx  # y*32

    jmp     addVbas  # passer au code commun des fonctions

EGAVret:    
    jmp     plotVEGAret

# code commun des routines plotVEGA

addVbas:    
    addw    %cx,%bx # y*(16+64) = y*80 ou y*(8+32) = y*40

    movw    %ax,%cx # cx = x
    shrw    $3,%cx  # x div 8

    addw    %cx,%bx # y*octet par ligne + x div 8
    movb    %dh,%ah # sauver la couleur
    movw    %bx,%dx

# masques du pixel

    andb    $7,%al  # x mod 8
    movb    $7,%cl  # cl = 7
    subb    %al,%cl # cl = 7 - x mod 8

    movb    $1,%bl  # 1 shl mask
    shlb    %cl,%bl
    movb    %bl,%bh # not (1 shl mask)
    notb    %bh

    movw    $4,%cx  # 4 plans a couvrir

# calcul de l'adresse complete du pixel de chaque plan
plotVplan:      
    ldsw    %es:(%di),%si  # adresse du plan dans le tableau
    addw    %dx,%si # ajout de l'offset de l'octet
    jnc     clearVbit

    pushw   %di
    movw    %es,%di # segment suivant
    addw    0x01000,%di    # arithmetique sur 20 bits
    movw    %di,%es
    popw    %di

# mise a 0 du bit

clearVbit:  
    andb    %bh, (%si)
    shlb    $1,%ah  # bit de couleur du plan
    jnc     nextplan

# mise a 1 du bit

    orb %bl, (%si)

nextplan:   addw    $4,%di  # pointeur de plan suivant
    jnc     gonextV    # 4 octets par adresse de plan

    movw    %es,%si # segment suivant
    addw    0x01000,%si    # arithmetique sur 20 bits
    movw    %si,%es

gonextV:    
    loop    plotVplan

# restaurer le contexte

    popw    %di
    popw    %si
    popw    %es
    popw    %dx
    popw    %cx
    popw    %bx
    popw    %ax

plotVEGAret:        
    ret 

#
# affiche un symbole rectangle bitmap
# sur ecran physique ou virtuel
#
# entree :
#
# AX = x
# BX = y
# CL = couleur de texte
# CH = couleur de fond
# DL = page
# DI = code du symbole dans la police
# DS:SI = adresse de la police bitmap
/*
.equ    SymX,   WORD PTR ss:[bp+2]
.equ    SymY,   WORD PTR ss:[bp+4]
.equ    SymText,    BYTE PTR ss:[bp+6]
.equ    SymFond,    BYTE PTR ss:[bp+7]
.equ    SymPage,    BYTE PTR ss:[bp+8]
.equ    SymLarg,    WORD PTR ss:[bp+10]
.equ    SymPol, WORD PTR ss:[bp+12]
.equ    SymCode,    WORD PTR ss:[bp+14]

.equ    PoliceAdr,  WORD PTR [si]
.equ    PoliceMax,  WORD PTR [si+4]
.equ    PoliceLarg, WORD PTR [si+6]
.equ    PoliceHaut, WORD PTR [si+8]
*/
.globl $plotEGA
plotEGA:

    push    %es # pour l'adresse de la routine plot
    pushw   %di # pour l'offset de l'octet du pixel
    pushw   %si # pour l'adresse de la routine plot
    pushw   8(%si) # variable locale : pix/lig
    pushw   %dx # DL pour la couleur de chaque pixel
    pushw   %cx # compteur de boucles
    pushw   %bx # compteur de lignes
    pushw   %ax # compteur de colonnes

    pushw   %bp
    movw    %sp,%bp

# verifier que c'est un mode supporte

    movw    $0x0F00,%ax # demande d'info sur le mode courant
    int     $0x10
    subb    %ah,%ah # largeur d'ecran inutile

tryEGA13:   
    movw    $plotEGA13,%si
    subw    $EGA13,%ax
    jz      plotsymok

tryEGA14:   
    movw    $plotEGA14,%si
    decw    %ax
    jz      plotsymok

    decw    %ax # mode 15 non supporte
    jz      plotsymret

tryEGA16:  
    movw    $plotEGA16,%si
    decw    %ax
    jnz     plotsymret

plotsymok:      
    movw    %si,%es # sauvergarde temporaire
    movw    %ss:12(%bp),%si # relire l'offset de la police

# calcul de l'adresse du motif
# par rapport a l'adresse de la table

    movw    %ss:14(%bp),%ax    # numero du symbole
    subw    %dx,%dx # poids fort du symbole (etendu)

    movw    4(%si),%cx  # pas au dela de 65535 symboles
    divw    %cx # numero modulo max
    xchgw   %di,%dx # retour en DI (le reste de la division)

# recherche de l'adresse dans le tableau de la structure

    shlw    $2,%di  # 4 octets par adresses pour les pointeurs long de matrice
    addw    (%si),%di  # base table + code * 4 (octet/adr)
    ldsw    (%di),%di   # la bonne adresse de matrice

    movw    8(%si),%cx # relire la hauteur de la police
    movw    %ss:2(%bp),%ax   # relire x
    movw    %ss:4(%bp),%bx   # relire y
    movw    %es,%si # relire l'adresse de la routine

plotsymlig: 
    pushw   %cx # sauver nblig
    pushw   %ax # sauver x base
    movw    %ss:10(%bp),%cx    # relire de pix/lig

plotsympix: 
    movb    (%di),%dl   # couleur du pixel

# mise a jour de la couleur effective
# 1 = texte
# 0 = fond

    movb    %ss:7(%bp),%dh    # couleur de fond
    orb     %dl,%dl # est-ce un fond ?
    je      symgetpage # c'est un fond
    movb    %ss:6(%bp),%dh    # couleur de texte

symgetpage: 
    movb    %ss:8(%bp),%dl    # relire la page

    callw   %si # afficher un pixel

    incw    %di # couleur suivante
    incw    %ax # une colonne de plus

    loop    plotsympix # pixel suivant

    popw    %ax # retrouver x base
    incw    %bx # une ligne de plus
    popw    %cx # retrouver nblig

    loop    plotsymlig # ligne suivante

# restaurer le contexte

plotsymret: 
    popw    %bp
    popw    %ax
    popw    %bx
    popw    %cx
    popw    %dx
    popw    %si
    popw    %si
    popw    %di
    popw    %es
    ret 


#
# fonction C printEGA (int x, int y, int coulfond, int coultxt,
# int code, police* custom, int page)
#
/*
.equ    X,  WORD PTR ss:[bp+4]
.equ    Y,  WORD PTR ss:[bp+6]
.equ    coulfond,   BYTE PTR ss:[bp+8]
.equ    coultxt,    BYTE PTR ss:[bp+10]
.equ    codesym,    WORD PTR ss:[bp+12]
.equ    custom, DWORD PTR ss:[bp+14]
.equ    codepage,   BYTE PTR ss:[bp+18]
*/
.globl $_printEGA
_printEGA:
 
    pushw   %bp
    movw    %sp,%bp

    pushw   %si
    pushw   %di
    push    %ds
    pushw   %ax
    pushw   %bx
    pushw   %cx
    pushw   %dx

    movw    %ss:12(%bp),%di
    movb    %ss:18(%bp),%dl   # page
    movb    %ss:10(%bp),%cl    # couleur de texte
    movb    %ss:8(%bp),%ch   # couleur de fond
    movw    %ss:6(%bp),%bx  # y
    movw    %ss:4(%bp),%ax  # x
    ldsw    %ss:14(%bp),%si # adresse de la police

plotsym:    
    call    plotEGA

    popw    %dx
    popw    %cx
    popw    %bx
    popw    %ax
    pop     %ds
    popw    %di
    popw    %si

    popw    %bp
    ret 


#
# setmode (int mode);
#
# mise en mode video
#

/*.equ    nouvmode,   BYTE PTR ss:[bp+4]*/

.globl  $_setmode
_setmode:

    pushw   %bp
    movw    %sp,%bp
    pushw   %ax

    movb    %ss:4(%bp),%al
    subb    %ah,%ah
    int     $0x10

    popw    %ax
    popw    %bp

    ret 


#
# int getmode (void);
#
# Lecture du mode video
#
# l'entier est rendu par AX
#
.globl  $_getmode
_getmode:

    pushw   %bp
    movw    %sp,%bp

    movw    $0x0F00,%ax
    int     $0x10
    subb    %ah,%ah # nombre de car/lig inutile

    popw    %bp

    ret 



#
# void actplan (int plan, int mode);
#
# activation d'un plan EGA
#
/*
.equ    plan,   BYTE PTR ss:[bp+4]
.equ    modeact,    BYTE PTR ss:[bp+6]
*/
_actplan:
    .globl  $_actplan

    pushw   %bp
    movw    %sp,%bp

    pushw   %ax
    pushw   %bx
    pushw   %cx
    pushw   %dx

# 0 lecture
# 1 fin lecture
# 2 ecriture
# 3 fin ecriture

# lecture

    movb    $4,%bl
    movw    $EGAGRAPH,%dx

    movb    %ss:4(%bp),%bh
    andb    $3,%bh

    movb    %ss:6(%bp),%ah
    andb    $3,%ah

    jz      newplan

# fin de lecture

    subb    %bh,%bh

    decb    %ah
    jz      newplan

# ecriture

    movb    $2,%bl
    movw    $EGASEQ,%dx

    movb    %ss:4(%bp),%cl
    andb    $3,%cl
    movb    $1,%bh
    shlb    %cl,%bh

    decb    %ah
    jz      newplan

# fin de lecture

    movb    $15,%bh

newplan:    
    movb    %bl,%al
    outb    %al,%dx
    incw    %dx
    movb    %bh,%al
    outb    %al,%dx

    popw    %dx
    popw    %cx
    popw    %bx
    popw    %ax
    popw    %bp
    ret 

#
# void activepage (int page);
#
# activation de la page
#
/*
.equ    nouvpage,   BYTE PTR ss:[bp+4]
*/
.globl  $_actpage
_actpage:

    pushw   %bp
    movw    %sp,%bp

    pushw   %ax

    movb    $5,%ah
    movb    %ss:4(%bp),%al
    int     $0x10

    popw    %ax
    popw    %bp
    ret 

#
# void kbdwait(void);
#
# attente de la frappe d'une touche
#
.globl  $_kbdwait
_kbdwait:

    pushw   %ax
    pushw   %dx

READKBD:        
    movb    $0x0ff,%dl
    movb    $6,%ah
    int     $0x21
    jz      READKBD

    cmpb    $27,%al
    je      ESCAPE

    popw    %dx
    popw    %ax
    clc 
    ret 

ESCAPE:     
    popw    %dx
    popw    %ax
    stc 
    ret 

#_TEXT:  ENDS    



