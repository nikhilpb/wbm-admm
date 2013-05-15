CC=gcc

all: wbm

#wbm-vec: wbm.o block.o main.o
#	$(CC) -pthread -O2 -DNOFUNCCALL -vec-report2 -o wbm-vec1.out main.c wbm.c block.c -I.

wbm: wbm.o block.o main.o
	$(CC) -pthread -O2 -o wbm.out main.c wbm.c block.c -I.
