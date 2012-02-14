CFLAGS=-Wall -Wstrict-prototypes -O2

9222cal: 9222cal.o conio.o
	gcc -o 9222cal -L/usr/lib -lpci_dask 9222cal.o conio.o

9222cal.o: 9222cal.c
	gcc -c $(CFLAGS) -I../../../include 9222cal.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 9222cal *.o
