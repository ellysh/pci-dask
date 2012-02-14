CFLAGS=-Wall -Wstrict-prototypes -O2

9116sstg: 9116sstg.o conio.o
	gcc -o 9116sstg -L/usr/lib -lpci_dask -lpthread 9116sstg.o conio.o

9116sstg.o: 9116sstg.c
	gcc -c $(CFLAGS) -I../../../include 9116sstg.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9116sstg 9116sstg.o conio.o 9116dat.txt
