CFLAGS=-Wall -Wstrict-prototypes -O2

9223ao: 9223ao.o conio.o
	gcc -o 9223ao -L/usr/lib -lpci_dask 9223ao.o conio.o

9223ao.o: 9223ao.c
	gcc -c $(CFLAGS) -I../../../include 9223ao.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 9223ao *.o
