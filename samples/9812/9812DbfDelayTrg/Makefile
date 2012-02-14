CFLAGS=-Wall -Wstrict-prototypes -O2

9812dltg: 9812dltg.o conio.o
	gcc -o 9812dltg -L/usr/lib -lpci_dask -lpthread 9812dltg.o conio.o

9812dltg.o: 9812dltg.c
	gcc -c $(CFLAGS) -I../../../include 9812dltg.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9812dltg 9812dltg.o conio.o 9812dat.txt
