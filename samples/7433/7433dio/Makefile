# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7433test: 7433test.o conio.o
	gcc -o 7433test -lpci_dask 7433test.o conio.o

7433test.o: 7433test.c
	gcc -c $(CFLAGS) 7433test.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f 7433test *.o *~
