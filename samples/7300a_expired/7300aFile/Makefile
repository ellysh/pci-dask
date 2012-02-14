CFLAGS=-Wall -Wstrict-prototypes -O2

7300afile: 7300afile.o conio.o
	gcc -o 7300afile -L/usr/lib -lpci_dask 7300afile.o conio.o

7300afile.o: 7300afile.c
	gcc -c $(CFLAGS) -I../../../include 7300afile.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c

clean:
	rm -f 7300afile 7300afile.o conio.o 7300a.txt
