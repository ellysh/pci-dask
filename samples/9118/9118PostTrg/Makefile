CFLAGS=-Wall -Wstrict-prototypes -O2

9118sptg: 9118sptg.o conio.o
	gcc -o 9118sptg -L/usr/lib -lpci_dask -lpthread 9118sptg.o conio.o

9118sptg.o: 9118sptg.c
	gcc -c $(CFLAGS) -I../../../include 9118sptg.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9118sptg 9118sptg.o conio.o 9118dat.txt
