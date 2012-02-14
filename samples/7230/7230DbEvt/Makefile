CFLAGS=-Wall -Wstrict-prototypes -O2

7230dbint: 7230dbint.o conio.o
	gcc -o 7230dbint -L/usr/lib -lpci_dask 7230dbint.o conio.o

7230dbint.o: 7230dbint.c
	gcc -c $(CFLAGS) -I../../../include 7230dbint.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7230dbint 7230dbint.o conio.o
