# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

6308ao: 6308ao.o conio.o
	gcc -o 6308ao -lpci_dask conio.o 6308ao.o

6308ao.o: 6308ao.c
	gcc -c $(CFLAGS) 6308ao.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f 6308ao *.o *~
