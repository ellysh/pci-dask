CFLAGS=-Wall -Wstrict-prototypes -O2

9812sdtg: 9812sdtg.o conio.o
	gcc -o 9812sdtg -L/usr/lib -lpci_dask -lpthread 9812sdtg.o conio.o

9812sdtg.o: 9812sdtg.c
	gcc -c $(CFLAGS) -I../../../include 9812sdtg.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9812sdtg 9812sdtg.o conio.o 9812dat.txt
