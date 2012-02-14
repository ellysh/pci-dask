CFLAGS=-Wall -Wstrict-prototypes -O2

9812sprg: 9812sprg.o conio.o
	gcc -o 9812sprg -L/usr/lib -lpci_dask -lpthread 9812sprg.o conio.o

9812sprg.o: 9812sprg.c
	gcc -c $(CFLAGS) -I../../../include 9812sprg.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9812sprg 9812sprg.o conio.o 9812dat.txt
