CFLAGS=-Wall -Wstrict-prototypes -O2

7233int: 7233int.o conio.o
	gcc -o 7233int -L/usr/lib -lpci_dask 7233int.o conio.o

7233int.o: 7233int.c
	gcc -c $(CFLAGS) -I../../../include 7233int.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7233int 7233int.o conio.o
