# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7300int: 7300int.o
	gcc -o 7300int -lpci_dask 7300int.o

7300int.o: 7300int.c
	gcc -c $(CFLAGS) 7300int.c

clean:
	rm -f 7300int *.o *~
