CFLAGS=-Wall -Wstrict-prototypes -O2

p9111ao: p9111ao.o conio.o
	gcc -o p9111ao -L/usr/lib -lpci_dask p9111ao.o conio.o

p9111ao.o: p9111ao.c
	gcc -c $(CFLAGS) -I../../../include p9111ao.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f p9111ao p9111ao.o conio.o
