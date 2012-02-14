CFLAGS=-Wall -Wstrict-prototypes -O2

7433dbint: 7433dbint.o conio.o
	gcc -o 7433dbint -L/usr/lib -lpci_dask 7433dbint.o conio.o

7433dbint.o: 7433dbint.c
	gcc -c $(CFLAGS) -I../../../include 7433dbint.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7433dbint 7433dbint.o conio.o
