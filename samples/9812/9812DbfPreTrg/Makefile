CFLAGS=-Wall -Wstrict-prototypes -O2

9812prtg: 9812prtg.o conio.o
	gcc -o 9812prtg -L/usr/lib -lpci_dask -lpthread 9812prtg.o conio.o

9812prtg.o: 9812prtg.c
	gcc -c $(CFLAGS) -I../../../include 9812prtg.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9812prtg 9812prtg.o conio.o 9812dat.txt
