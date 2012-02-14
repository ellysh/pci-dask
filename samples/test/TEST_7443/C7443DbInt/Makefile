CFLAGS=-Wall -Wstrict-prototypes -O2

7443dbint: 7443dbint.o conio.o
	gcc -o 7443dbint -L/usr/lib -lpci_dask 7443dbint.o conio.o

7443dbint.o: 7443dbint.c
	gcc -c $(CFLAGS) -I../../../include 7443dbint.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c
	
clean:
	rm -f 7443dbint  7443dbint.o conio.o
