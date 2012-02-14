CFLAGS=-Wall -Wstrict-prototypes -O2

9114dbf: 9114dbf.o conio.o
	gcc -o 9114dbf -L/usr/lib -lpci_dask -lpthread 9114dbf.o conio.o

9114dbf.o: 9114dbf.c
	gcc -c $(CFLAGS) -I../../../include 9114dbf.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9114dbf 9114dbf.o conio.o 9114dat.txt
