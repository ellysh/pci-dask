CFLAGS=-Wall -Wstrict-prototypes -O2

9114dio: 9114dio.o conio.o
	gcc -o 9114dio -L/usr/lib -lpci_dask 9114dio.o conio.o

9114dio.o: 9114dio.c
	gcc -c $(CFLAGS) -I../../../include 9114dio.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9114dio 9114dio.o conio.o
