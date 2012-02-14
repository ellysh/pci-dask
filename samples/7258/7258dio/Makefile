# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7258test: 7258test.o conio.o
	gcc -o 7258test -lpci_dask 7258test.o conio.o

7258test.o: 7258test.c
	gcc -c $(CFLAGS) 7258test.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f *.o *~ 7258test
