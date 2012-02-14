CFLAGS=-Wall -Wstrict-prototypes -O2

7200cwav: 7200cwav.o conio.o
	gcc -o 7200cwav -L/usr/lib -lpci_dask -lpthread -lm $(CFLAGS) 7200cwav.o conio.o

7200cwav.o: 7200cwav.c
	gcc -c $(CFLAGS) -I../../../include 7200cwav.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7200cwav 7200cwav.o conio.o 7200d.dat
