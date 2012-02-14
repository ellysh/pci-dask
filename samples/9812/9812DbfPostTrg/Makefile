CFLAGS=-Wall -Wstrict-prototypes -O2

9812ptrg: 9812ptrg.o conio.o
	gcc -o 9812ptrg -L/usr/lib -lpci_dask -lpthread 9812ptrg.o conio.o

9812ptrg.o: 9812ptrg.c
	gcc -c $(CFLAGS) -I../../../include 9812ptrg.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9812ptrg 9812ptrg.o conio.o 9812dat.txt
