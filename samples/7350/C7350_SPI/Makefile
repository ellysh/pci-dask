# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

C7350_SPI: C7350_SPI.o conio.o
	gcc -o C7350_SPI -lpci_dask C7350_SPI.o conio.o

C7350_SPI.o: C7350_SPI.c
	gcc -c $(CFLAGS) C7350_SPI.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f C7350_SPI *.o
