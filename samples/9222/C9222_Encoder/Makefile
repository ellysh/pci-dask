CFLAGS=-Wall -Wstrict-prototypes -O2

9222ecd: 9222ecd.o conio.o
	gcc -o 9222ecd -L/usr/lib -lpci_dask 9222ecd.o conio.o

9222ecd.o: 9222ecd.c
	gcc -c $(CFLAGS) -I../../../include 9222ecd.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 9222ecd *.o
