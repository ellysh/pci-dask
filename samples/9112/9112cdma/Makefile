CFLAGS= -Wall -Wstrict-prototypes -O2

9112cdma: 9112cdma.o conio.o
	gcc -o 9112cdma -L/usr/lib -lpci_dask -lpthread 9112cdma.o conio.o

9112cdma.o: 9112cdma.c
	gcc -c $(CFLAGS) -I../../../include 9112cdma.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9112cdma 9112cdma.o conio.o 9112dat.txt
