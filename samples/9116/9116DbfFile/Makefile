CFLAGS=-Wall -Wstrict-prototypes -O2

9116dbf: 9116dbf.o conio.o
	gcc -o 9116dbf -L/usr/lib -lpci_dask -lpthread 9116dbf.o conio.o

9116dbf.o: 9116dbf.c
	gcc -c $(CFLAGS) -I../../../include 9116dbf.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9116dbf 9116dbf.o conio.o 9116dat.txt
