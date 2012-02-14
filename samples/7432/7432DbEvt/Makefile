CFLAGS=-Wall -Wstrict-prototypes -O2

7432dbint: 7432dbint.o conio.o
	gcc -o 7432dbint -L/usr/lib -lpci_dask 7432dbint.o conio.o

7432dbint.o: 7432dbint.c
	gcc -c $(CFLAGS) -I../../../include 7432dbint.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7432dbint 7432dbint.o conio.o
