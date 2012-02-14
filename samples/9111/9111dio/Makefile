CFLAGS=-Wall -Wstrict-prototypes -O2

9111dio: 9111dio.o conio.o
	gcc -o 9111dio -L/usr/lib -lpci_dask 9111dio.o conio.o

9111dio.o: 9111dio.c
	gcc -c $(CFLAGS) -I../../../include 9111dio.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9111dio 9111dio.o conio.o
