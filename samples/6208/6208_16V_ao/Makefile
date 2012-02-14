# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

6208ao: 6208ao.o  conio.o
	gcc -o 6208ao -lpci_dask conio.o 6208ao.o

6208ao.o: 6208ao.c
	gcc -c $(CFLAGS) 6208ao.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f 6208ao *.o *~
