CFLAGS=-Wall -Wstrict-prototypes -O2

9222ao: 9222ao.o conio.o
	gcc -o 9222ao -L/usr/lib -lpci_dask 9222ao.o conio.o

9222ao.o: 9222ao.c
	gcc -c $(CFLAGS) -I../../../include 9222ao.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 9222ao *.o
