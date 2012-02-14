CFLAGS=-Wall -Wstrict-prototypes -O2

9222dio: 9222dio.o conio.o
	gcc -o 9222dio -L/usr/lib -lpci_dask 9222dio.o conio.o

9222dio.o: 9222dio.c
	gcc -c $(CFLAGS) -I../../../include 9222dio.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 9222dio *.o
