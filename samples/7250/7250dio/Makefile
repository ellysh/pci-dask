# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7250test: 7250test.o conio.o
	gcc -o 7250test -lpci_dask 7250test.o conio.o

7250test.o: 7250test.c
	gcc -c $(CFLAGS) 7250test.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f 7250test *.o *~
