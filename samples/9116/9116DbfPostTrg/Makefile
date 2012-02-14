CFLAGS=-Wall -Wstrict-prototypes -O2

9116ptrg: 9116ptrg.o conio.o
	gcc -o 9116ptrg -L/usr/lib -lpci_dask -lpthread 9116ptrg.o conio.o

9116ptrg.o: 9116ptrg.c
	gcc -c $(CFLAGS) -I../../../include 9116ptrg.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9116ptrg 9116ptrg.o conio.o 9116dat.txt
