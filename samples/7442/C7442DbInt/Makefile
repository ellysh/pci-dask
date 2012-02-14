CFLAGS=-Wall -Wstrict-prototypes -O2

7442dbint: 7442dbint.o conio.o
	gcc -o 7442dbint -L/usr/lib -lpci_dask 7442dbint.o conio.o

7442dbint.o: 7442dbint.c
	gcc -c $(CFLAGS) -I../../../include 7442dbint.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7442dbint  7442dbint.o conio.o
