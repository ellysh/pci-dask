CFLAGS=-Wall -Wstrict-prototypes -O2

9223cal: 9223cal.o conio.o
	gcc -o 9223cal -L/usr/lib -lpci_dask 9223cal.o conio.o

9223cal.o: 9223cal.c
	gcc -c $(CFLAGS) -I../../../include 9223cal.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 9223cal *.o
