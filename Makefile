CC=icc

all: wbm-vec wbm

wbm-vec: wbm.o wbm-aux.o main.o
	$(CC) -pthread -O2 -DNOFUNCCALL -vec-report2 -o wbm-vec1.out main.c wbm.c wbm-aux.c -I.

wbm: wbm.o wbm-aux.o main.o
	$(CC) -pthread -O2 -o wbm.out main.c wbm.c wbm-aux.c -I.
