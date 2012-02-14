CFLAGS=-Wall -Wstrict-prototypes -O2

7452int: 7452int.o conio.o
	gcc -o 7452int -L/usr/lib -lpci_dask 7452int.o conio.o

7452int.o: 7452int.c
	gcc -c $(CFLAGS) -I../../../include 7452int.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7452int 7452int.o conio.o
