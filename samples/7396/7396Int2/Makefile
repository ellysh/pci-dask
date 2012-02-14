# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7396int2: 7396int2.o
	gcc -o 7396int2 -lpci_dask 7396int2.o

7396int2.o: 7396int2.c
	gcc -c $(CFLAGS) 7396int2.c

clean:
	rm -f 7396int2 *.o *~
