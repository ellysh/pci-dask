CFLAGS=-Wall -Wstrict-prototypes -O2

p9118ao: p9118ao.o conio.o
	gcc -o p9118ao -L/usr/lib -lpci_dask p9118ao.o conio.o

p9118ao.o: p9118ao.c
	gcc -c $(CFLAGS) -I../../../include p9118ao.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f p9118ao p9118ao.o conio.o
