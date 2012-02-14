CFLAGS=-Wall -Wstrict-prototypes -O2

9118atrg: 9118atrg.o conio.o
	gcc -o 9118atrg -L/usr/lib -lpci_dask -lpthread 9118atrg.o conio.o

9118atrg.o: 9118atrg.c
	gcc -c $(CFLAGS) -I../../../include 9118atrg.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9118atrg 9118atrg.o conio.o 9118dat.txt
