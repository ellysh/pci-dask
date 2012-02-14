CFLAGS=-Wall -Wstrict-prototypes -O2

9812sptg: 9812sptg.o conio.o
	gcc -o 9812sptg -L/usr/lib -lpci_dask -lpthread 9812sptg.o conio.o

9812sptg.o: 9812sptg.c
	gcc -c $(CFLAGS) -I../../../include 9812sptg.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9812sptg 9812sptg.o conio.o 9812dat.txt
