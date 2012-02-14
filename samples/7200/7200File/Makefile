CFLAGS=-Wall -Wstrict-prototypes -O2

7200file: 7200file.o conio.o
	gcc -o 7200file -L/usr/lib -lpci_dask $(CFLAGS) 7200file.o conio.o

7200file.o: 7200file.c
	gcc -c $(CFLAGS) -I../../../include 7200file.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7200file 7200file.o conio.o 7200d.dat
