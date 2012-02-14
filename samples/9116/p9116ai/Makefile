CFLAGS=-Wall -Wstrict-prototypes -O2

p9116ai: p9116ai.o conio.o
	gcc -o p9116ai -L/usr/lib -lpci_dask p9116ai.o conio.o

p9116ai.o: p9116ai.c
	gcc -c $(CFLAGS) -I../../../include p9116ai.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f p9116ai p9116ai.o conio.o
