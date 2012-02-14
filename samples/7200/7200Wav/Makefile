CFLAGS=-Wall -Wstrict-prototypes -O2

7200wav: 7200wav.o conio.o
	gcc -o 7200wav -L/usr/lib -lpci_dask -lm 7200wav.o conio.o

7200wav.o: 7200wav.c
	gcc -c $(CFLAGS) -I../../../include 7200wav.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7200wav 7200wav.o conio.o 7200d.dat
