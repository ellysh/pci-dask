# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7248int: 7248int.o
	gcc -o 7248int -lpci_dask 7248int.o

7248int.o: 7248int.c
	gcc -c $(CFLAGS) 7248int.c

clean:
	rm -f 7248int *.o *~
