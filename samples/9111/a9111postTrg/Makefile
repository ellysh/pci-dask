CFLAGS=-Wall -Wstrict-prototypes -O2

a9111file: a9111file.o conio.o
	gcc -o a9111file -L/usr/lib -lpci_dask a9111file.o conio.o

a9111file.o: a9111file.c
	gcc -c $(CFLAGS) -I../../../include a9111file.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f a9111file a9111file.o conio.o 9111dat.txt .directory
