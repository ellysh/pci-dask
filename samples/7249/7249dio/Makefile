# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7249test: 7249test.o conio.o
	gcc -o 7249test -lpci_dask 7249test.o conio.o

7249test.o: 7249test.c
	gcc -c $(CFLAGS) 7249test.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f 7249test *.o *~
