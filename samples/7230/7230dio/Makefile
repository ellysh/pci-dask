# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7230test: 7230test.o conio.o
	gcc -o 7230test -lpci_dask 7230test.o conio.o

7230test.o: 7230test.c
	gcc -c $(CFLAGS) 7230test.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAFS) ../../conio/conio.c

clean:
	rm -f 7230test *.o *~
