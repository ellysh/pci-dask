CFLAGS=-Wall -Wstrict-prototypes -O2

7300awav: 7300awav.o conio.o
	gcc -o 7300awav -L/usr/lib -lpci_dask -lm 7300awav.o conio.o

7300awav.o: 7300awav.c
	gcc -c $(CFLAGS) -I../../../include 7300awav.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7300awav 7300awav.o conio.o 7300b.txt


