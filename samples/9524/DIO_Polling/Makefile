CFLAGS=-Wall -Wstrict-prototypes -O2

9524dio: 9524dio.o conio.o
	gcc -o 9524dio -L/usr/lib -lpci_dask 9524dio.o conio.o

9524dio.o: 9524dio.c
	gcc -c $(CFLAGS) -I../../../include 9524dio.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 9524dio *.o
