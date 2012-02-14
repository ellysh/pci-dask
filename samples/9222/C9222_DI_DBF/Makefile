CFLAGS=-Wall -Wstrict-prototypes -O2

9222di: 9222di.o conio.o
	gcc -o 9222di -L/usr/lib -lpci_dask 9222di.o conio.o

9222di.o: 9222di.c
	gcc -c $(CFLAGS) -I../../../include 9222di.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 9222di *.o *.dat
