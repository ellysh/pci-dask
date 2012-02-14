CFLAGS=-Wall -Wstrict-prototypes -O2

7200hdsk: 7200hdsk.o conio.o
	gcc -o 7200hdsk -L/usr/lib -lpci_dask -lm 7200hdsk.o conio.o

7200hdsk.o: 7200hdsk.c
	gcc -c $(CFLAGS) -I../../../include 7200hdsk.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7200hdsk 7200hdsk.o conio.o 7200d.dat
