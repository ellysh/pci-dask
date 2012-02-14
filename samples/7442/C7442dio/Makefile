CFLAGS=-Wall -Wstrict-prototypes -O2

CDIOOnePoint: CDIOOnePoint.o conio.o
	gcc -o CDIOOnePoint -L/usr/lib -lpci_dask CDIOOnePoint.o conio.o

CDIOOnePoint.o: CDIOOnePoint.c
	gcc -c $(CFLAGS) -I../../../include CDIOOnePoint.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f CDIOOnePoint CDIOOnePoint.o conio.o
