CFLAGS=-Wall -Wstrict-prototypes -O2

7300bpgwav: 7300bpgwav.o conio.o
	gcc -o 7300bpgwav -L/usr/lib -lpci_dask -lm 7300bpgwav.o conio.o

7300bpgwav.o: 7300bpgwav.c
	gcc -c $(CFLAGS) -I../../../include 7300bpgwav.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7300bpgwav 7300bpgwav.o conio.o 7300b.dat


