CFLAGS=-Wall -Wstrict-prototypes -O2

9116file: 9116file.o conio.o
	gcc -o 9116file -L/usr/lib -lpci_dask -lpthread 9116file.o conio.o

9116file.o: 9116file.c
	gcc -c $(CFLAGS) -I../../../include 9116file.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9116file 9116file.o conio.o 9116dat.txt
