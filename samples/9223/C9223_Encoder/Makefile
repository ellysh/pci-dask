CFLAGS=-Wall -Wstrict-prototypes -O2

9223ecd: 9223ecd.o conio.o
	gcc -o 9223ecd -L/usr/lib -lpci_dask 9223ecd.o conio.o

9223ecd.o: 9223ecd.c
	gcc -c $(CFLAGS) -I../../../include 9223ecd.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 9223ecd *.o
