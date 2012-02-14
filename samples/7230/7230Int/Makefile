CFLAGS=-Wall -Wstrict-prototypes -O2

7230int: 7230int.o conio.o
	gcc -o 7230int -L/usr/lib -lpci_dask 7230int.o conio.o

7230int.o: 7230int.c
	gcc $(CFLAGS) -c -I../../../include 7230int.c

conio.o: ../../conio/conio.c
	gcc $(CFLAGS) -c -I../../../include ../../conio/conio.c

clean:
	rm -f 7230int 7230int.o conio.o
