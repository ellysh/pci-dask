CFLAGS=-Wall -Wstrict-prototypes -O2

9524ai: 9524ai.o conio.o
	gcc -o 9524ai -L/usr/lib -lpci_dask 9524ai.o conio.o

9524ai.o: 9524ai.c
	gcc -c $(CFLAGS) -I../../../include 9524ai.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 9524ai *.o acq.dat
