# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7256test: 7256test.o conio.o
	gcc -o 7256test -lpci_dask 7256test.o conio.o

7256test.o: 7256test.c
	gcc -c $(CFLAGS) 7256test.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f 7256test *.o *~
