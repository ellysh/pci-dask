CFLAGS=-Wall -Wstrict-prototypes -O2

p9113ai: p9113ai.o conio.o
	gcc -o p9113ai -L/usr/lib -lpci_dask p9113ai.o conio.o

p9113ai.o: p9113ai.c
	gcc -c $(CFLAGS) -I../../../include p9113ai.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f p9113ai p9113ai.o conio.o
