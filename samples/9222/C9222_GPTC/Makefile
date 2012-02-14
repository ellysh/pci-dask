CFLAGS=-Wall -Wstrict-prototypes -O2

9222gptc: 9222gptc.o conio.o
	gcc -o 9222gptc -L/usr/lib -lpci_dask 9222gptc.o conio.o

9222gptc.o: 9222gptc.c
	gcc -c $(CFLAGS) -I../../../include 9222gptc.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 9222gptc *.o
