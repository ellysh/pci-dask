CFLAGS=-Wall -Wstrict-prototypes -O2

9222do: 9222do.o conio.o
	gcc -o 9222do -L/usr/lib -lpci_dask 9222do.o conio.o

9222do.o: 9222do.c
	gcc -c $(CFLAGS) -I../../../include 9222do.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 9222do *.o
