# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7234test: 7234test.o conio.o
	gcc -o 7234test -lpci_dask 7234test.o conio.o

7234test.o: 7234test.c
	gcc -c $(CFLAGS) 7234test.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f 7234test *.o *~
