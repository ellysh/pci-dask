CFLAGS=-Wall -Wstrict-prototypes -O2

9812mtrg: 9812mtrg.o conio.o
	gcc -o 9812mtrg -L/usr/lib -lpci_dask -lpthread 9812mtrg.o conio.o

9812mtrg.o: 9812mtrg.c
	gcc -c $(CFLAGS) -I../../../include 9812mtrg.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9812mtrg 9812mtrg.o conio.o 9812dat.txt
