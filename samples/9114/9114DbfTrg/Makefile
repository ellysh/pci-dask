CFLAGS=-Wall -Wstrict-prototypes -O2

9114PrTg: 9114PrTg.o conio.o
	gcc -o 9114PrTg -L/usr/lib -lpci_dask -lpthread 9114PrTg.o conio.o

9114PrTg.o: 9114PrTg.c
	gcc -c $(CFLAGS) -I../../../include 9114PrTg.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9114PrTg 9114PrTg.o conio.o 9114dat.txt
