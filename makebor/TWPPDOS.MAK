.AUTODEPEND

.PATH.obj = .

#		*Translator Definitions*
CC = tcc +TWPPDOS.CFG
TASM = TASM
TLIB = tlib
TLINK = tlink
LIBPATH = H:\PROGLANG\TC\LIB
INCLUDEPATH = H:\PROGLANG\TC\INCLUDE


#		*Implicit Rules*
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

#		*List Macros*


EXE_dependencies =  \
 twremaps.obj \
 abends.obj \
 globals.obj \
 twpair.obj \
 twport.obj \
 twppfind.obj \
 twsector.obj \
 twwarp.obj

#		*Explicit Rules*
.\twppdos.exe: twppdos.cfg $(EXE_dependencies)
  $(TLINK) /x/c/L$(LIBPATH) @&&|
c0c.obj+
.\twremaps.obj+
.\abends.obj+
.\globals.obj+
.\twpair.obj+
.\twport.obj+
.\twppfind.obj+
.\twsector.obj+
.\twwarp.obj
.\twppdos
		# no map file
emu.lib+
mathc.lib+
cc.lib
|


#		*Individual File Dependencies*
twremaps.obj: twppdos.cfg twremaps.cpp 

abends.obj: twppdos.cfg abends.cpp 

globals.obj: twppdos.cfg globals.cpp 

twpair.obj: twppdos.cfg twpair.cpp 

twport.obj: twppdos.cfg twport.cpp 

twppfind.obj: twppdos.cfg twppfind.cpp 

twsector.obj: twppdos.cfg twsector.cpp 

twwarp.obj: twppdos.cfg twwarp.cpp 

#		*Compiler Configuration File*
twppdos.cfg: twppdos.mak
  copy &&|
-mc
-N
-G
-O
-Z
-vi-
-H=TWPPDOS.SYM
-w-ret
-w-nci
-w-inl
-wpin
-wamb
-wamp
-w-par
-wasm
-wcln
-w-cpt
-wdef
-w-dup
-w-pia
-wsig
-wnod
-w-ill
-w-sus
-wstv
-wucp
-wuse
-w-ext
-w-ias
-w-ibc
-w-pre
-w-nst
-n.
-I$(INCLUDEPATH)
-L$(LIBPATH)
-Ff
| twppdos.cfg


