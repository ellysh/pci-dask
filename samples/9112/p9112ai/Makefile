CFLAGS=-Wall -Wstrict-prototypes -O2

p9112ai: p9112ai.o conio.o
	gcc -o p9112ai -L/usr/lib -lpci_dask p9112ai.o conio.o

p9112ai.o: p9112ai.c
	gcc -c $(CFLAGS) -I../../../include p9112ai.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f p9112ai p9112ai.o conio.o
