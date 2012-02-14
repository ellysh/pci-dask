# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

dio: dio.o conio.o
	gcc -o dio -lpci_dask dio.o conio.o

dio.o: dio.c
	gcc -c $(CFLAGS) dio.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f dio *.o
