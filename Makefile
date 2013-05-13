CC=gcc
OFLAGS="-O2 -pthread" 

wbm: wbm.o wbm-aux.o
	$(CC) -pthread -O2 -o wbm.out wbm.c wbm-aux.c -I.
