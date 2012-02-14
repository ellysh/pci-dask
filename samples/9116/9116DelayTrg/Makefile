CFLAGS=-Wall -Wstrict-prototypes -O2

9116sdtg: 9116sdtg.o conio.o
	gcc -o 9116sdtg -L/usr/lib -lpci_dask -lpthread 9116sdtg.o conio.o

9116sdtg.o: 9116sdtg.c
	gcc -c $(CFLAGS) -I../../../include 9116sdtg.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9116sdtg 9116sdtg.o conio.o 9116dat.txt
