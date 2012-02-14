CFLAGS=-Wall -Wstrict-prototypes -O2

p9118ai: p9118ai.o conio.o
	gcc -o p9118ai -L/usr/lib -lpci_dask p9118ai.o conio.o

p9118ai.o: p9118ai.c
	gcc -c $(CFLAGS) -I../../../include p9118ai.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f p9118ai p9118ai.o conio.o
