CFLAGS=-Wall -Wstrict-prototypes -O2

9812sstg: 9812sstg.o conio.o
	gcc -o 9812sstg -L/usr/lib -lpci_dask -lpthread 9812sstg.o conio.o

9812sstg.o: 9812sstg.c
	gcc -c $(CFLAGS) -I../../../include 9812sstg.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9812sstg 9812sstg.o conio.o 9812dat.txt
