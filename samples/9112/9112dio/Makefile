CFLAGS=-Wall -Wstrict-prototypes -O2

9112dio: 9112dio.o conio.o
	gcc -o 9112dio -L/usr/lib -lpci_dask 9112dio.o conio.o

9112dio.o: 9112dio.c
	gcc -c $(CFLAGS) -I../../../include 9112dio.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9112dio 9112dio.o conio.o
