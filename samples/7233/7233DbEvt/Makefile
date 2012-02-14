CFLAGS=-Wall -Wstrict-prototypes -O2

7233dbint: 7233dbint.o conio.o
	gcc -o 7233dbint -L/usr/lib -lpci_dask 7233dbint.o conio.o

7233dbint.o: 7233dbint.c
	gcc $(CFLAGS) -c -I../../../include 7233dbint.c

conio.o: ../../conio/conio.c
	gcc $(CFLAGS) -c -I../../../include ../../conio/conio.c

clean:
	rm -f 7233dbint 7233dbint.o conio.o
