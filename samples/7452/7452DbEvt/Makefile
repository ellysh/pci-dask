CFLAGS=-Wall -Wstrict-prototypes -O2

7452dbint: 7452dbint.o conio.o
	gcc -o 7452dbint -L/usr/lib -lpci_dask 7452dbint.o conio.o

7452dbint.o: 7452dbint.c
	gcc -c $(CFLAGS) -I../../../include 7452dbint.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7452dbint 7452dbint.o conio.o
