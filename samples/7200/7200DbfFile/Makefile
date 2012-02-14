CFLAGS= -Wall -Wstrict-prototypes -O2

7200dbf: 7200dbf.o conio.o
	gcc -o 7200dbf -L/usr/lib -lpci_dask -lpthread 7200dbf.o conio.o

7200dbf.o: 7200dbf.c
	gcc -c $(CFLAGS) -I../../../include 7200dbf.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7200dbf 7200dbf.o conio.o 7200d.dat
