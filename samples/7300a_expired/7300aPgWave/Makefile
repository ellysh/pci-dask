CFLAGS=-Wall -Wstrict-prototypes -O2

7300apgwav: 7300apgwav.o conio.o
	gcc -o 7300apgwav -L/usr/lib -lpci_dask -lm 7300apgwav.o conio.o

7300apgwav.o: 7300apgwav.c
	gcc -c $(CFLAGS) -I../../../include 7300apgwav.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7300apgwav 7300apgwav.o conio.o 7300b.txt

