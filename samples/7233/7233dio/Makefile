# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7233test: 7233test.o conio.o
	gcc -o 7233test -lpci_dask 7233test.o conio.o

7233test.o: 7233test.c
	gcc -c $(CFLAGS) 7233test.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f 7233test *.o *~
