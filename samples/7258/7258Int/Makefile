CFLAGS=-Wall -Wstrict-prototypes -O2

7258Int: 7258Int.o conio.o
	gcc -o 7258Int -L/usr/lib -lpci_dask 7258Int.o conio.o

7258Int.o: 7258Int.c
	gcc -c $(CFLAGS) -I../../../include 7258Int.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7258Int.o conio.o 7258Int
