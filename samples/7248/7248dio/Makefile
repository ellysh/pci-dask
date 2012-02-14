# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7248test: 7248test.o conio.o
	gcc -o 7248test -lpci_dask 7248test.o conio.o

7248test.o: 7248test.c
	gcc -c $(CFLAGS) 7248test.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f 7248test *.o *~
