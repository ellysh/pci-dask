# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7249dbint: 7249dbint.o
	gcc -o 7249dbint -lpci_dask 7249dbint.o

7249dbint.o: 7249dbint.c
	gcc -c $(CFLAGS) 7249dbint.c

clean:
	rm -f 7249dbint *.o *~
