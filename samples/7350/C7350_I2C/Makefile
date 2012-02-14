# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

C7350_I2C: C7350_I2C.o conio.o
	gcc -o C7350_I2C -lpci_dask C7350_I2C.o conio.o

C7350_I2C.o: C7350_I2C.c
	gcc -c $(CFLAGS) C7350_I2C.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f C7350_I2C *.o
