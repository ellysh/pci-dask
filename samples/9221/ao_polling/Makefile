CFLAGS=-Wall -Wstrict-prototypes -O2

9221ao: 9221ao.o conio.o
	gcc -o 9221ao -L/usr/lib -lpci_dask 9221ao.o conio.o

9221ao.o: 9221ao.c
	gcc -c $(CFLAGS) -I../../../include 9221ao.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 9221ao *.o
