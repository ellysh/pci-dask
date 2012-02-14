CFLAGS=-Wall -Wstrict-prototypes -O2

9223gptc: 9223gptc.o conio.o
	gcc -o 9223gptc -L/usr/lib -lpci_dask 9223gptc.o conio.o

9223gptc.o: 9223gptc.c
	gcc -c $(CFLAGS) -I../../../include 9223gptc.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 9223gptc *.o
