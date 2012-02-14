# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

6202test: 6202test.o conio.o
	gcc -o 6202test -lpci_dask 6202test.o  conio.o

6202test.o: 6202test.c
	gcc -c $(CFLAGS) 6202test.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f 6202test *.o
