CFLAGS=-Wall -Wstrict-prototypes -O2

9221dio: 9221dio.o conio.o
	gcc -o 9221dio -L/usr/lib -lpci_dask 9221dio.o conio.o

9221dio.o: 9221dio.c
	gcc -c $(CFLAGS) -I../../../include 9221dio.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 9221dio *.o
