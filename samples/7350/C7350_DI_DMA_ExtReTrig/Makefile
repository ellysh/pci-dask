# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

di: di.o conio.o
	gcc -o di -lpci_dask di.o conio.o

di.o: di.c
	gcc -c $(CFLAGS) di.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f di *.o
