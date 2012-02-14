CFLAGS=-Wall -Wstrict-prototypes -O2

9114test: 9114test.o conio.o
	gcc -o 9114test -L/usr/lib -lpci_dask -lpthread 9114test.o conio.o

9114test.o: 9114test.c
	gcc -c $(CFLAGS) -I../../../include 9114test.c
	
conio.o: conio.c
	gcc -c -I../../../include conio.c

clean:
	rm -f 9114test 9114test.o conio.o
