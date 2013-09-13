# makefile
# Created by IBM WorkFrame/2 MakeMake at 19:37:05 on 10 Jan 1999
#
# The actions included in this make file are:
#  Compile::C++ Compiler
#  Link::Linker

.SUFFIXES:

.SUFFIXES: \
    .cpp .obj 

.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Tl10 /Su4 /Sp1 /Ss /Q /Fi /Si /O /G5 /Gf /Gi /Gx /Ft- /Gu /C %s

{h:\twports}.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Tl10 /Su4 /Sp1 /Ss /Q /Fi /Si /O /G5 /Gf /Gi /Gx /Ft- /Gu /C %s

all: \
    .\twppos2.exe

.\twppos2.exe: \
    .\TWSector.obj \
    .\TWWarp.obj \
    .\AbEnds.obj \
    .\Globals.obj \
    .\TWPair.obj \
    .\TWPort.obj \
    .\TWPPFind.obj \
    .\TWRemaps.obj
    @echo " Link::Linker "
    icc.exe @<<
     /Q /B" /exepack:1 /packd /optfunc      /noe"
     /Fetwppos2.exe 
     .\TWSector.obj
     .\TWWarp.obj
     .\AbEnds.obj
     .\Globals.obj
     .\TWPair.obj
     .\TWPort.obj
     .\TWPPFind.obj
     .\TWRemaps.obj
<<

.\TWSector.obj: \
    h:\twports\TWSector.cpp \
    {h:\twports;$(INCLUDE);}TWWarp.hpp \
    {h:\twports;$(INCLUDE);}TWTypes.hpp \
    {h:\twports;$(INCLUDE);}Globals.hpp \
    {h:\twports;$(INCLUDE);}AbEnds.hpp \
    {h:\twports;$(INCLUDE);}TWPPFind.h \
    {h:\twports;$(INCLUDE);}TWSector.hpp \
    {h:\twports;$(INCLUDE);}TWPort.hpp \
    {h:\twports;$(INCLUDE);}TWPair.hpp \
    {h:\twports;$(INCLUDE);}TWRemaps.hpp

.\TWRemaps.obj: \
    h:\twports\TWRemaps.cpp

.\TWPPFind.obj: \
    h:\twports\TWPPFind.cpp \
    {h:\twports;$(INCLUDE);}TWWarp.hpp \
    {h:\twports;$(INCLUDE);}TWTypes.hpp \
    {h:\twports;$(INCLUDE);}Globals.hpp \
    {h:\twports;$(INCLUDE);}AbEnds.hpp \
    {h:\twports;$(INCLUDE);}TWPPFind.h \
    {h:\twports;$(INCLUDE);}TWSector.hpp \
    {h:\twports;$(INCLUDE);}TWPort.hpp \
    {h:\twports;$(INCLUDE);}TWPair.hpp \
    {h:\twports;$(INCLUDE);}TWRemaps.hpp

.\TWPort.obj: \
    h:\twports\TWPort.cpp \
    {h:\twports;$(INCLUDE);}TWTypes.hpp \
    {h:\twports;$(INCLUDE);}TWPort.hpp

.\TWPair.obj: \
    h:\twports\TWPair.cpp \
    {h:\twports;$(INCLUDE);}TWTypes.hpp \
    {h:\twports;$(INCLUDE);}Globals.hpp \
    {h:\twports;$(INCLUDE);}AbEnds.hpp \
    {h:\twports;$(INCLUDE);}TWSector.hpp \
    {h:\twports;$(INCLUDE);}TWPort.hpp \
    {h:\twports;$(INCLUDE);}TWPair.hpp

.\Globals.obj: \
    h:\twports\Globals.cpp \
    {h:\twports;$(INCLUDE);}TWTypes.hpp \
    {h:\twports;$(INCLUDE);}Globals.hpp \
    {h:\twports;$(INCLUDE);}TWSector.hpp \
    {h:\twports;$(INCLUDE);}TWPort.hpp \
    {h:\twports;$(INCLUDE);}TWPair.hpp

.\AbEnds.obj: \
    h:\twports\AbEnds.cpp \
    {h:\twports;$(INCLUDE);}TWTypes.hpp \
    {h:\twports;$(INCLUDE);}Globals.hpp \
    {h:\twports;$(INCLUDE);}AbEnds.hpp \
    {h:\twports;$(INCLUDE);}TWPPFind.h

.\TWWarp.obj: \
    h:\twports\TWWarp.cpp \
    {h:\twports;$(INCLUDE);}TWWarp.hpp \
    {h:\twports;$(INCLUDE);}TWTypes.hpp
