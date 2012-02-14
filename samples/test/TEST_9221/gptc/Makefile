CFLAGS=-Wall -Wstrict-prototypes -O2

gptc: gptc.o conio.o
	gcc -o gptc -L/usr/lib -lpci_dask gptc.o conio.o

gptc.o: gptc.c
	gcc -c $(CFLAGS) -I../../../include gptc.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c
	
clean:
	rm -f gptc *.o 
