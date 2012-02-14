CFLAGS=-Wall -Wstrict-prototypes -O2

CDIOnePoint: CDIOnePoint.o conio.o
	gcc -o CDIOnePoint -L/usr/lib -lpci_dask CDIOnePoint.o conio.o

CDIOnePoint.o: CDIOnePoint.c
	gcc -c $(CFLAGS) -I../../../include CDIOnePoint.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c
	
clean:
	rm -f CDIOnePoint CDIOnePoint.o conio.o
