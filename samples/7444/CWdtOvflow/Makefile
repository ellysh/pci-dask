CFLAGS=-Wall -Wstrict-prototypes -O2

WdtOvflow: WdtOvflow.o conio.o
	gcc -o WdtOvflow -L/usr/lib -lpci_dask WdtOvflow.o conio.o

WdtOvflow.o: WdtOvflow.c
	gcc -c $(CFLAGS) -I../../../include WdtOvflow.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f WdtOvflow WdtOvflow.o conio.o
