CC=gcc
OFLAGS=-O2

wbm: wbm.o wbm-aux.o
	$(CC) $(OFLAGS) -o wbm.out wbm.c wbm-aux.c -I.
