CFLAGS=-Wall -Wstrict-prototypes -O2

a9114file: a9114file.o conio.o
	gcc -o a9114file -L/usr/lib -lpci_dask a9114file.o conio.o

a9114file.o: a9114file.c
	gcc -c $(CFLAGS) -I../../../include a9114file.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f a9114file a9114file.o conio.o 9114dat.txt
