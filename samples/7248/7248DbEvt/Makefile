# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7248dbevent: 7248dbevent.o
	gcc -o 7248dbevent -lpci_dask 7248dbevent.o

7248dbevent.o: 7248dbevent.c
	gcc -c $(CFLAGS) 7248dbevent.c

clean:
	rm -f 7248dbevent *.o *~
