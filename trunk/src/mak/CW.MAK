.AUTODEPEND

#		*Translator Definitions*
CC = tcc +CW.CFG
TASM = TASM
TLINK = tlink


#		*Implicit Rules*
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

.asm.obj:
  $(TASM) {$< }


#		*List Macros*


EXE_dependencies =  \
  codobj.obj \
  core.obj \
  dicoetiq.obj \
  dicoires.obj \
  exec.obj \
  hach.obj \
  instruct.obj \
  listprog.obj \
  load.obj \
  mars.obj \
  msgerr.obj \
  stat.obj \
  tradfic.obj \
  tradlig.obj \
  visu.obj \
  ega.obj

#		*Explicit Rules*
cw.exe: cw.cfg $(EXE_dependencies)
  $(TLINK) /v/x/i/n/c @&&|
c:\tc\lib\c0c.obj+
codobj.obj+
core.obj+
dicoetiq.obj+
dicoires.obj+
exec.obj+
hach.obj+
instruct.obj+
listprog.obj+
load.obj+
mars.obj+
msgerr.obj+
stat.obj+
tradfic.obj+
tradlig.obj+
visu.obj+
ega.obj
cw,
c:\tc\lib\emu.lib+
c:\tc\lib\mathc.lib+
c:\tc\lib\cc.lib
|


#		*Individual File Dependencies*
codobj.obj: codobj.c 

core.obj: core.c 

dicoetiq.obj: dicoetiq.c 

dicoires.obj: dicoires.c 

exec.obj: exec.c 

hach.obj: hach.c 

instruct.obj: instruct.c 

listprog.obj: listprog.c 

load.obj: load.c 

mars.obj: mars.c 

msgerr.obj: msgerr.c 

stat.obj: stat.c 

tradfic.obj: tradfic.c 

tradlig.obj: tradlig.c 

visu.obj: visu.c 

ega.obj: ega.asm 
	$(TASM) ega.asm

#		*Compiler Configuration File*
cw.cfg: cw.mak
  copy &&|
-mc
-1
-ff-
-v
-G
-V
-vi-
-wamb
-wamp
-wasm
-wpro
-wcln
-wdef
-wsig
-wnod
-wstv
-wucp
-wuse
-IC:\TC\INCLUDE
-LC:\TC\LIB
-LC:\TEXTES\C\LICENCE\COREWAR\WORK
| cw.cfg


