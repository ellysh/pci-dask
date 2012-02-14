CFLAGS=-Wall -Wstrict-prototypes -O2

9118satg: 9118satg.o conio.o
	gcc -o 9118satg -L/usr/lib -lpci_dask -lpthread 9118satg.o conio.o

9118satg.o: 9118satg.c
	gcc -c $(CFLAGS) -I../../../include 9118satg.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9118satg 9118satg.o conio.o 9118dat.txt
