SYSTEM     = x86-64_sles10_4.1
LIBFORMAT  = static_pic

#------------------------------------------------------------
#
# When you adapt this makefile to compile your CPLEX programs
# please copy this makefile and set CPLEXDIR and CONCERTDIR to
# the directories where CPLEX and CONCERT are installed.
#
#------------------------------------------------------------

CPLEXDIR      = /apps/cplex/cplex
CONCERTDIR    = /apps/cplex/concert
# ---------------------------------------------------------------------
# Compiler selection 
# ---------------------------------------------------------------------

CCC = g++
CC  = gcc
JAVAC = javac

# ---------------------------------------------------------------------
# Compiler options 
# ---------------------------------------------------------------------

CCOPT = -m64 -O -fPIC -fexceptions -DNDEBUG -DIL_STD
COPT  = -m64 -O2 -fPIC 

CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXJARDIR   = $(CPLEXDIR)/lib/cplex.jar
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

CCLNFLAGS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lm -pthread
CLNFLAGS  = -L$(CPLEXLIBDIR) -lcplex -lm -pthread

CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include

CFLAGS  = $(COPT)  -I$(CPLEXINCDIR)
CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) 

WBMFLG = -O2 -pthread
LSTDFLG = -lstdc++ -lm -lgsl -lgslcblas
INCLUDE = -I/usr/include/ 
LIB = -L/usr/lib/
OBJS = lp wbm

all:	${OBJS}
	rm -f *.o

lp.o:	lp.cpp
	$(CCC) -c $(CCFLAGS) lp.cpp -o lp.o ${INCLUDE} -I/usr/local/include -pthread

lp:	lp.o
	$(CCC) $(CCFLAGS) lp.o -o wbm-cplex.out ${LIB} ${LSTDFLG} $(CCLNFLAGS)

wbm: wbm.o block.o main.o
	$(CC) $(CFLAGS) -o wbm-admm.out main.c wbm.c block.c -I. $(CLNFLAGS)

clean:
	rm -f *.o
	rm -f ${OBJS}

