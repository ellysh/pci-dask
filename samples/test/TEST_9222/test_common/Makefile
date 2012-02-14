CFLAGS=-Wall -Wstrict-prototypes -O2

common: common.o conio.o
	gcc -o common -L/usr/lib -lpci_dask common.o conio.o

common.o: common.c
	gcc -c $(CFLAGS) -I../../../include common.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../conio ../../conio/conio.c

clean:
	rm -f common *.o acq*
