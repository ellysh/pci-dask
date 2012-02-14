# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

6308test: 6308test.o conio.o
	gcc -o 6308test -lpci_dask 6308test.o conio.o

6308test.o: 6308test.c
	gcc -c $(CFLAGS) 6308test.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f 6308test *.o *~
