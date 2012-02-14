CFLAGS=-Wall -Wstrict-prototypes -O2

Util7444: Util7444.o conio.o
	gcc -o Util7444 -L/usr/lib -lpci_dask Util7444.o conio.o

Util7444.o: Util7444.c
	gcc -c $(CFLAGS) -I../../../include Util7444.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f Util7444 Util7444.o conio.o
