CFLAGS=-Wall -Wstrict-prototypes -O2

ctr: ctr.o conio.o
	gcc -o ctr -L/usr/lib -lpci_dask ctr.o conio.o

ctr.o: ctr.c
	gcc -c $(CFLAGS) -I../../../include ctr.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f ctr *.o
