CFLAGS=-Wall -Wstrict-prototypes -O2

p9111ai: p9111ai.o conio.o
	gcc -o p9111ai -L/usr/lib -lpci_dask p9111ai.o conio.o

p9111ai.o: p9111ai.c
	gcc -c $(CFLAGS) -I../../../include p9111ai.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f p9111ai p9111ai.o conio.o
