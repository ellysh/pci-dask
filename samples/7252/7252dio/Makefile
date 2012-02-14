# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7252test: 7252test.o conio.o
	gcc -o 7252test -lpci_dask 7252test.o conio.o

7252test.o: 7252test.c
	gcc -c $(CFLAGS) 7252test.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f 7252test *.o *~
