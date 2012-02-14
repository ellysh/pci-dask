CFLAGS=-Wall -Wstrict-prototypes -O2

7200ETrig: 7200ETrig.o
	gcc -o 7200ETrig -L/usr/lib -lpci_dask $(CFLAGS) 7200ETrig.o

7200ETrig.o: 7200ETrig.c
	gcc -c $(CFLAGS) -I../../../include 7200ETrig.c

clean:
	rm -f 7200ETrig 7200ETrig.o
