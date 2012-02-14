# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7432test: 7432test.o conio.o
	gcc -o 7432test -lpci_dask 7432test.o conio.o

7432test.o: 7432test.c
	gcc -c $(CFLAGS) 7432test.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f 7432test *.o *~
