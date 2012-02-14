CFLAGS=-Wall -Wstrict-prototypes -O2

9111file: 9111file.o conio.o
	gcc -o 9111file -L/usr/lib -lpci_dask 9111file.o conio.o

9111file.o: 9111file.c
	gcc -c $(CFLAGS) -I../../../include 9111file.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9111file 9111file.o conio.o 9111dat.txt
