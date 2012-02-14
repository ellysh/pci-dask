CFLAGS=-Wall -Wstrict-prototypes -O2

7256Cos: 7256Cos.o conio.o
	gcc -o 7256Cos -L/usr/lib -lpci_dask 7256Cos.o conio.o

7256Cos.o: 7256Cos.c
	gcc -c $(CFLAGS) -I../../../include 7256Cos.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7256Cos 7256Cos.o conio.o
