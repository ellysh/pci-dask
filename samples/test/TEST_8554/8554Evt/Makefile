CFLAGS=-Wall -Wstrict-prototypes -O2

8554evt: 8554evt.o conio.o
	gcc -o 8554evt -L/usr/lib -lpci_dask 8554evt.o conio.o

8554evt.o: 8554evt.c
	gcc -c $(CFLAGS) -I../../../include 8554evt.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c
	
clean:
	rm -f 8554evt 8554evt.o conio.o
