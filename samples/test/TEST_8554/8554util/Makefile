CFLAGS=-Wall -Wstrict-prototypes -O2

8554util: 8554util.o conio.o
	gcc -o 8554util -L/usr/lib -lpci_dask 8554util.o conio.o

8554util.o: 8554util.c
	gcc -c $(CFLAGS) -I../../../include 8554util.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 8554util 8554util.o conio.o
