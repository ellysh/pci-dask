CFLAGS=-Wall -Wstrict-prototypes -O2

p9114ai: p9114ai.o conio.o
	gcc -o p9114ai -L/usr/lib -lpci_dask p9114ai.o conio.o

p9114ai.o: p9114ai.c
	gcc -c $(CFLAGS) -I../../../include p9114ai.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f p9114ai p9114ai.o conio.o
