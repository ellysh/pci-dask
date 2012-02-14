# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7396cosi: 7396cosi.o conio.o
	gcc -o 7396cosi -lpci_dask 7396cosi.o conio.o

7396cosi.o: 7396cosi.c
	gcc -c $(CFLAGS) 7396cosi.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 7396cosi *.o *~
