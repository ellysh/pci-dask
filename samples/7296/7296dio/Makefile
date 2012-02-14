# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7296test: 7296test.o conio.o
	gcc -o 7296test -lpci_dask 7296test.o conio.o

7296test.o: 7296test.c
	gcc -c $(CFLAGS) 7296test.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f 7296test *.o *~
