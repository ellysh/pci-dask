CFLAGS=-Wall -Wstrict-prototypes -O2

9222ai: 9222ai.o conio.o
	gcc -o 9222ai -L/usr/lib -lpci_dask 9222ai.o conio.o

9222ai.o: 9222ai.c
	gcc -c $(CFLAGS) -I../../../include 9222ai.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 9222ai *.o *.dat *.cvt
