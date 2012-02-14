CFLAGS=-Wall -Wstrict-prototypes -O2

9812file: 9812file.o conio.o
	gcc -o 9812file -L/usr/lib -lpci_dask -lpthread 9812file.o conio.o

9812file.o: 9812file.c
	gcc -c $(CFLAGS) -I../../../include 9812file.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9812file 9812file.o conio.o 9812dat.txt
