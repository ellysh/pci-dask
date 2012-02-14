CFLAGS= -Wall -O2

9112dma: 9112dma.o
	gcc -o 9112dma -L/usr/lib -lpci_dask $(CFLAGS) 9112dma.o

9112dma.o: 9112dma.c
	gcc $(CFLAGS) -c -I../../../include 9112dma.c

clean:
	rm -f 9112dma 9112dma.o 9112dat.txt
