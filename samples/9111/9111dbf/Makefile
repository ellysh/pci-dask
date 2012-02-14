CFLAGS=-Wall -Wstrict-prototypes -O2

9111dbf: 9111dbf.o conio.o
	gcc -o 9111dbf -L/usr/lib -lpci_dask -lpthread 9111dbf.o conio.o

9111dbf.o: 9111dbf.c
	gcc -c $(CFLAGS) -I../../../include 9111dbf.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9111dbf 9111dbf.o conio.o 9111dat.txt
