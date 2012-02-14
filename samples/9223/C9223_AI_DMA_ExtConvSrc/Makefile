CFLAGS=-Wall -Wstrict-prototypes -O2

9223ai: 9223ai.o conio.o
	gcc -o 9223ai -L/usr/lib -lpci_dask 9223ai.o conio.o

9223ai.o: 9223ai.c
	gcc -c $(CFLAGS) -I../../../include 9223ai.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 9223ai *.o *.dat
