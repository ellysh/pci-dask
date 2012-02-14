CFLAGS=-Wall -Wstrict-prototypes -O2

9114file: 9114file.o conio.o
	gcc -o 9114file -L/usr/lib -lpci_dask 9114file.o conio.o

9114file.o: 9114file.c
	gcc -c $(CFLAGS) -I../../../include 9114file.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9114file 9114file.o conio.o 9114dat.txt
