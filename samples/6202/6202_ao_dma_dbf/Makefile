CFLAGS=-Wall -Wstrict-prototypes -O2

6202ao: 6202ao.o conio.o
	gcc -o 6202ao -L/usr/lib -lpci_dask 6202ao.o conio.o

6202ao.o: 6202ao.c
	gcc -c $(CFLAGS) -I../../../include 6202ao.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f 6202ao *.o
