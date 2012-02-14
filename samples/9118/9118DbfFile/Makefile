CFLAGS=-Wall -Wstrict-prototypes -O2

9118dbf: 9118dbf.o conio.o
	gcc -o 9118dbf -L/usr/lib -lpci_dask -lpthread 9118dbf.o conio.o

9118dbf.o: 9118dbf.c
	gcc -c $(CFLAGS) -I../../../include 9118dbf.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9118dbf 9118dbf.o conio.o 9118dat.txt
