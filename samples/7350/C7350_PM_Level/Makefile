# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

patternmatch: patternmatch.o conio.o
	gcc -o patternmatch -lpci_dask patternmatch.o conio.o

patternmatch.o: patternmatch.c
	gcc -c $(CFLAGS) patternmatch.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f patternmatch *.o
