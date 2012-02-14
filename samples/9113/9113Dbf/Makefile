CFLAGS=-Wall -Wstrict-prototypes -O2

9113dbf: 9113dbf.o conio.o
	gcc -o 9113dbf -L/usr/lib -lpci_dask -lpthread 9113dbf.o conio.o

9113dbf.o: 9113dbf.c
	gcc -c $(CFLAGS) -I../../../include 9113dbf.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9113dbf 9113dbf.o conio.o 9113dat.txt
