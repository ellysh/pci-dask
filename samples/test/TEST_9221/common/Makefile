CFLAGS=-Wall -Wstrict-prototypes -O2

9221_Common: 9221_Common.o
	gcc -o 9221_Common -L/usr/lib -lpci_dask 9221_Common.o

9221_Common.o: 9221_Common.c
	gcc -c $(CFLAGS) -I../../../include -I../../../../include 9221_Common.c

	
clean:
	rm -f 9221_Common *.o 
