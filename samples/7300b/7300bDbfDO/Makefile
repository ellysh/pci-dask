CFLAGS=-Wall -Wstrict-prototypes -O2

7300bDbf: 7300bDbf.o conio.o
	gcc -o 7300bDbf -L/usr/lib -lpci_dask -lm 7300bDbf.o conio.o

7300bDbf.o: 7300bDbf.c
	gcc -c $(CFLAGS) -I../../../include 7300bDbf.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7300bDbf 7300bDbf.o conio.o
