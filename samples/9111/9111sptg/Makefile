CFLAGS=-Wall -Wstrict-prototypes -O2

9111sptg: 9111sptg.o conio.o
	gcc -o 9111sptg -L/usr/lib -lpci_dask 9111sptg.o conio.o

9111sptg.o: 9111sptg.c
	gcc -c $(CFLAGS) -I../../../include 9111sptg.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9111sptg 9111sptg.o conio.o 9111dat.txt
