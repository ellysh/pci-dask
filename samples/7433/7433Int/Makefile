CFLAGS=-Wall -Wstrict-prototypes -O2

7433int: 7433int.o conio.o
	gcc -o 7433int -L/usr/lib -lpci_dask 7433int.o conio.o

7433int.o: 7433int.c
	gcc -c $(CFLAGS) -I../../../include 7433int.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 7433int 7433int.o conio.o
