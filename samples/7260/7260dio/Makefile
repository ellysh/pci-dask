# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7260test: 7260test.o conio.o
	gcc -o 7260test -lpci_dask 7260test.o conio.o

7260test.o: 7260test.c
	gcc -c $(CFLAGS) 7260test.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f 7260test *.o *~
