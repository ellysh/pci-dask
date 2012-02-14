# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7296int: 7296int.o
	gcc -o 7296int -lpci_dask 7296int.o

7296int.o: 7296int.c
	gcc -c $(CFLAGS) 7296int.c

clean:
	rm -f 7296int *.o *~
