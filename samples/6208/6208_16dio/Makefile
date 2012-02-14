# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

6208test: 6208test.o conio.o
	gcc -o 6208test -lpci_dask 6208test.o  conio.o

6208test.o: 6208test.c
	gcc -c $(CFLAGS) 6208test.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f 6208test *.o *~
