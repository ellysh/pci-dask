CFLAGS=-Wall -Wstrict-prototypes -O2

9116smtg: 9116smtg.o conio.o
	gcc -o 9116smtg -L/usr/lib -lpci_dask -lpthread 9116smtg.o conio.o

9116smtg.o: 9116smtg.c
	gcc -c $(CFLAGS) -I../../../include 9116smtg.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9116smtg 9116smtg.o conio.o 9116dat.txt
