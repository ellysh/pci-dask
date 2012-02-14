CFLAGS=-Wall -Wstrict-prototypes -O2

9118dio: 9118dio.o conio.o
	gcc -o 9118dio -L/usr/lib -lpci_dask 9118dio.o conio.o

9118dio.o: 9118dio.c
	gcc -c $(CFLAGS) -I../../../include 9118dio.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9118dio 9118dio.o conio.o
