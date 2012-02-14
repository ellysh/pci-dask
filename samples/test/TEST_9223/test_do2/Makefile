CFLAGS=-Wall -Wstrict-prototypes -O2

9223dio: 9223dio.o conio.o
	gcc -o 9223dio -L/usr/lib -lpci_dask 9223dio.o conio.o

9223dio.o: 9223dio.c
	gcc -c $(CFLAGS) -I../../../include 9223dio.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 9223dio *.o
