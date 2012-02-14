CFLAGS=-Wall -Wstrict-prototypes -O2

9118prtg: 9118prtg.o conio.o
	gcc -o 9118prtg -L/usr/lib -lpci_dask -lpthread 9118prtg.o conio.o

9118prtg.o: 9118prtg.c
	gcc -c $(CFLAGS) -I../../../include 9118prtg.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9118prtg 9118prtg.o conio.o 9118dat.txt
