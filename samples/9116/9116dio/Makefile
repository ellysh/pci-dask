CFLAGS=-Wall -Wstrict-prototypes -O2

9116dio: 9116dio.o conio.o
	gcc -o 9116dio -L/usr/lib -lpci_dask 9116dio.o conio.o

9116dio.o: 9116dio.c
	gcc -c $(CFLAGS) -I../../../include 9116dio.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9116dio 9116dio.o conio.o
