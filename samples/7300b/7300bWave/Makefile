CFLAGS=-Wall -Wstrict-prototypes -O2

7300bwav: 7300bwav.o conio.o
	gcc -o 7300bwav -L/usr/lib -lpci_dask -lm 7300bwav.o conio.o

7300bwav.o: 7300bwav.c
	gcc -c $(CFLAGS) -I../../../include 7300bwav.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7300bwav 7300bwav.o conio.o 7300b.dat


