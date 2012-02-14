CFLAGS=-Wall -Wstrict-prototypes -O2

Util7442: Util7442.o conio.o
	gcc -o Util7442 -L/usr/lib -lpci_dask Util7442.o conio.o

Util7442.o: Util7442.c
	gcc -c $(CFLAGS) -I../../../include Util7442.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f Util7442 Util7442.o conio.o
