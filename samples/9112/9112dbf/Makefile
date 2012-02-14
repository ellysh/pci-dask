CFLAGS=-Wall -Wstrict-prototypes -O2

9112dbf: 9112dbf.o conio.o
	gcc -o 9112dbf -L/usr/lib -lpci_dask -lpthread 9112dbf.o conio.o

9112dbf.o: 9112dbf.c
	gcc -c $(CFLAGS) -I../../../include 9112dbf.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9112dbf 9112dbf.o conio.o 9112dat.txt
