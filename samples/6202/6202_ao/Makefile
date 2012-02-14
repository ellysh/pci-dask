# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

6202_ao: 6202_ao.o  conio.o
	gcc -o 6202_ao -lpci_dask conio.o 6202_ao.o

6202_ao.o: 6202_ao.c
	gcc -c $(CFLAGS) 6202_ao.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f 6202_ao *.o *~
