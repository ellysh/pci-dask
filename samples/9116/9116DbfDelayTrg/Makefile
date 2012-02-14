CFLAGS=-Wall -Wstrict-prototypes -O2

9116dltg: 9116dltg.o conio.o
	gcc -o 9116dltg -L/usr/lib -lpci_dask -lpthread 9116dltg.o conio.o

9116dltg.o: 9116dltg.c
	gcc -c $(CFLAGS) -I../../../include 9116dltg.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9116dltg 9116dltg.o conio.o 9116dat.txt
