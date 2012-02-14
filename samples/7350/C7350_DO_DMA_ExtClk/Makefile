# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

do: do.o conio.o
	gcc -o do -lpci_dask do.o conio.o

do.o: do.c
	gcc -c $(CFLAGS) do.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f do *.o
