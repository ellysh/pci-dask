# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

7296dbevent: 7296dbevent.o
	gcc -o 7296dbevent -lpci_dask 7296dbevent.o

7296dbevent.o: 7296dbevent.c
	gcc -c $(CFLAGS) 7296dbevent.c

clean:
	rm -f 7296dbevent *.o *~
