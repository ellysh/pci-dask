CFLAGS=-Wall -Wstrict-prototypes -O2

CDOOnePoint: CDOOnePoint.o conio.o
	gcc -o CDOOnePoint -L/usr/lib -lpci_dask CDOOnePoint.o conio.o

CDOOnePoint.o: CDOOnePoint.c
	gcc -c $(CFLAGS) -I../../../include CDOOnePoint.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f CDOOnePoint CDOOnePoint.o conio.o
