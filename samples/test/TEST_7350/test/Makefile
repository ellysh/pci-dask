CFLAGS=-Wall -Wstrict-prototypes -O2

test: test.o conio.o
	gcc -o test -L/usr/lib -lpci_dask test.o conio.o

test.o: test.c
	gcc -c $(CFLAGS) -I../../../include test.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../conio ../../conio/conio.c

clean:
	rm -f test *.o acq* 
