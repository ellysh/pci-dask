# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7434test: 7434test.o conio.o
	gcc -o 7434test -lpci_dask 7434test.o conio.o

7434test.o: 7434test.c
	gcc -c $(CFLAGS) 7434test.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f 7434test *.o *~
