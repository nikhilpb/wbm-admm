CC=gcc
OFLAGS=-O2

wbm: wbm-serial.o wbm-aux.o
	$(CC) $(OFLAGS) -o wbm-serial wbm-serial.c wbm-aux.c -I.
