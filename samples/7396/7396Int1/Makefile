# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7396int1: 7396int1.o conio.o
	gcc -o 7396int1 -lpci_dask 7396int1.o conio.o

7396int1.o: 7396int1.c
	gcc -c $(CFLAGS) 7396int1.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 7396int1 *.o *~
