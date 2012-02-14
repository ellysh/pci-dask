CFLAGS=-Wall -Wstrict-prototypes -O2

Util7443: Util7443.o conio.o
	gcc -o Util7443 -L/usr/lib -lpci_dask Util7443.o conio.o

Util7443.o: Util7443.c
	gcc -c $(CFLAGS) -I../../../include Util7443.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f Util7443 Util7443.o conio.o
