CFLAGS=-Wall -Wstrict-prototypes -O2

9118ptrg: 9118ptrg.o conio.o
	gcc -o 9118ptrg -L/usr/lib -lpci_dask -lpthread 9118ptrg.o conio.o

9118ptrg.o: 9118ptrg.c
	gcc -c $(CFLAGS) -I../../../include 9118ptrg.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9118ptrg 9118ptrg.o conio.o 9118dat.txt
