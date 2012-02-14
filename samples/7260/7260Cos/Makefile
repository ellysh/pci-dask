CFLAGS=-Wall -Wstrict-prototypes -O2

7260Cos: 7260Cos.o conio.o
	gcc -o 7260Cos -L/usr/lib -lpci_dask 7260Cos.o conio.o

7260Cos.o: 7260Cos.c
	gcc -c $(CFLAGS) -I../../../include 7260Cos.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7260Cos 7260Cos.o conio.o
