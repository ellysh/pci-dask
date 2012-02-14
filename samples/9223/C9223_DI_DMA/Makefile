CFLAGS=-Wall -Wstrict-prototypes -O2

9223di: 9223di.o conio.o
	gcc -o 9223di -L/usr/lib -lpci_dask 9223di.o conio.o

9223di.o: 9223di.c
	gcc -c $(CFLAGS) -I../../../include 9223di.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 9223di *.o *.dat
