CFLAGS=-Wall -Wstrict-prototypes -O2

9118file: 9118file.o conio.o
	gcc -o 9118file -L/usr/lib -lpci_dask -lpthread 9118file.o conio.o

9118file.o: 9118file.c
	gcc -c $(CFLAGS) -I../../../include 9118file.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9118file 9118file.o conio.o 9118dat.txt
