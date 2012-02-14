# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7249int: 7249int.o
	gcc -o 7249int -lpci_dask 7249int.o

7249int.o: 7249int.c
	gcc -c $(CFLAGS) 7249int.c

clean:
	rm -f 7249int *.o *~
