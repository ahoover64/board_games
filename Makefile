#
# Created by makemake (Darwin May 30 2016) on Sat Jul 30 12:49:35 2016
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .cc .s
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.cc.o:
		$(COMPILE.cc) $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cc.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		clang
CXX =		clang++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c

########## Default flags (redefine these with a header.mak file if desired)
CXXFLAGS =	-ggdb -Wall -pedantic
CFLAGS =	-ggdb -Wall -pedantic
BINDIR =./bin
CLIBFLAGS =	-lm
CCLIBFLAGS =	
########## End of default flags


CPP_FILES =	Amazons.cpp Amazons_AI.cpp Chess.cpp Game.cpp Neural_Net.cpp Player.cpp TicTacToe.cpp test.cpp trainer.cpp
C_FILES =	
S_FILES =	
H_FILES =	Amazons.h Amazons_AI.h Chess.h Game.h Neural_Net.h Player.h TicTacToe.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	Amazons.o Amazons_AI.o Chess.o Game.o Neural_Net.o Player.o TicTacToe.o 

#
# Main targets
#

all:	${BINDIR}/test ${BINDIR}/trainer 

${BINDIR}/test:	test.o $(OBJFILES)
	@mkdir -p ${BINDIR}/
	$(CXX) $(CXXFLAGS) -o ${BINDIR}/test test.o $(OBJFILES) $(CCLIBFLAGS)

${BINDIR}/trainer:	trainer.o $(OBJFILES)
	@mkdir -p ${BINDIR}/
	$(CXX) $(CXXFLAGS) -o ${BINDIR}/trainer trainer.o $(OBJFILES) $(CCLIBFLAGS)

#
# Dependencies
#

Amazons.o:	Amazons.h Game.h Player.h
Amazons_AI.o:	Amazons_AI.h Game.h Player.h
Chess.o:	
Game.o:	Game.h Player.h
Neural_Net.o:	Neural_Net.h
Player.o:	Player.h
TicTacToe.o:	Game.h Player.h TicTacToe.h
test.o:	Amazons.h Game.h Player.h TicTacToe.h
trainer.o:	Amazons.h Amazons_AI.h Game.h Player.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm $(OBJFILES) test.o trainer.o core 2> /dev/null

realclean:        clean
	-/bin/rm -rf ${BINDIR}/test ${BINDIR}/trainer 
