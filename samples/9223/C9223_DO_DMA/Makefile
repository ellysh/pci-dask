CFLAGS=-Wall -Wstrict-prototypes -O2

9223do: 9223do.o conio.o
	gcc -o 9223do -L/usr/lib -lpci_dask 9223do.o conio.o

9223do.o: 9223do.c
	gcc -c $(CFLAGS) -I../../../include 9223do.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 9223do *.o
