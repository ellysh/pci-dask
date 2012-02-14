CFLAGS=-Wall -Wstrict-prototypes -O2

9524ao: 9524ao.o conio.o
	gcc -o 9524ao -L/usr/lib -lpci_dask 9524ao.o conio.o

9524ao.o: 9524ao.c
	gcc -c $(CFLAGS) -I../../../include 9524ao.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 9524ao *.o
