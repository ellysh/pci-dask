CFLAGS=-Wall -Wstrict-prototypes -O2

9116sptg: 9116sptg.o conio.o
	gcc -o 9116sptg -L/usr/lib -lpci_dask -lpthread 9116sptg.o conio.o

9116sptg.o: 9116sptg.c
	gcc -c $(CFLAGS) -I../../../include 9116sptg.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9116sptg 9116sptg.o conio.o 9116dat.txt
