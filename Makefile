CC=icc
OFLAGS="-O2 -pthread" 

# wbm: wbm-join.o wbm-aux.o
# 	$(CC) -pthread -O2 -o wbm-join.out wbm-join.c wbm-aux.c -I.

wbm-vec: wbm.o wbm-aux.o
	$(CC) -pthread -O2 -DNOFUNCCALL -vec-report2 -o wbm.out wbm.c wbm-aux.c -I.

wbm: wbm.o wbm-aux.o
	$(CC) -pthread -O2 -o wbm.out wbm.c wbm-aux.c -I.

