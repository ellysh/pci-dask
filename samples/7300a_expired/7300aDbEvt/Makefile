# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include
                                                     
7300dbevent: 7300dbevent.o
	gcc -o 7300dbevent -lpci_dask 7300dbevent.o

7300dbevent.o: 7300dbevent.c
	gcc -c $(CFLAGS) 7300dbevent.c

clean:
	rm -f 7300dbevent *.o *~
