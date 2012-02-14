CFLAGS=-Wall -Wstrict-prototypes -O2

7300mwav: 7300mwav.o conio.o
	gcc -o 7300mwav -L/usr/lib -lpci_dask -lpthread -lm $(CFLAGS) 7300mwav.o conio.o

7300mwav.o: 7300mwav.c
	gcc -c $(CFLAGS) -I../../../include 7300mwav.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7300mwav 7300mwav.o conio.o 7300d.dat
