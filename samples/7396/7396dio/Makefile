# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7396test: 7396test.o conio.o
	gcc -o 7396test -lpci_dask 7396test.o conio.o

7396test.o: 7396test.c
	gcc -c $(CFLAGS) 7396test.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f 7396test *.o *~
