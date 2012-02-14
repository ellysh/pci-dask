CFLAGS=-Wall -Wstrict-prototypes -O2

7300bfile: 7300bfile.o conio.o
	gcc -o 7300bfile -L/usr/lib -lpci_dask 7300bfile.o conio.o

7300bfile.o: 7300bfile.c
	gcc -c $(CFLAGS) -I../../../include 7300bfile.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7300bfile 7300bfile.o conio.o 7300b.dat
