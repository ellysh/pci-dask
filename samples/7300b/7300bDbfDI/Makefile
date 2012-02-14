CFLAGS=-Wall -Wstrict-prototypes -O2

7300mdi: 7300mdi.o conio.o
	gcc -o 7300mdi -L/usr/lib -lpci_dask -lpthread -lm $(CFLAGS) 7300mdi.o conio.o

7300mdi.o: 7300mdi.c
	gcc -c $(CFLAGS) -I../../../include 7300mdi.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7300mdi 7300mdi.o conio.o 7300b.dat
