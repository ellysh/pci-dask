CFLAGS=-Wall -Wstrict-prototypes -O2

9812smtg: 9812smtg.o conio.o
	gcc -o 9812smtg -L/usr/lib -lpci_dask -lpthread 9812smtg.o conio.o

9812smtg.o: 9812smtg.c
	gcc -c $(CFLAGS) -I../../../include 9812smtg.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9812smtg 9812smtg.o conio.o 9812dat.txt
