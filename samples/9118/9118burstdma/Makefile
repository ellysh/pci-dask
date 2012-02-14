CFLAGS=-Wall -Wstrict-prototypes -O2

9118bdma: 9118bdma.o conio.o
	gcc -o 9118bdma -L/usr/lib -lpci_dask -lpthread 9118bdma.o conio.o

9118bdma.o: 9118bdma.c
	gcc -c $(CFLAGS) -I../../../include 9118bdma.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9118bdma 9118bdma.o conio.o 9118dat.txt
