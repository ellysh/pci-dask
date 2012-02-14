CFLAGS=-Wall -Wstrict-prototypes -O2

7432int: 7432int.o conio.o
	gcc -o 7432int -L/usr/lib -lpci_dask 7432int.o conio.o

7432int.o: 7432int.c
	gcc -c $(CFLAGS) -I../../../include 7432int.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7432int 7432int.o conio.o
