# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7396dbevent: 7396dbevent.o
	gcc -o 7396dbevent -lpci_dask 7396dbevent.o

7396dbevent.o: 7396dbevent.c
	gcc -c $(CFLAGS) 7396dbevent.c

clean:
	rm -f 7396dbevent *.o *~
