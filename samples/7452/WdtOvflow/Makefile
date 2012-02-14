# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7452test: 7452test.o conio.o
	gcc -o 7452test -lpci_dask 7452test.o conio.o

7452test.o: 7452test.c
	gcc -c $(CFLAGS) 7452test.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f 7452test *.o *~
