# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

cos: cos.o conio.o
	gcc -o cos -lpci_dask cos.o conio.o

cos.o: cos.c
	gcc -c $(CFLAGS) cos.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f cos *.o
