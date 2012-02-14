CFLAGS=-Wall -Wstrict-prototypes -O2

9113file: 9113file.o conio.o
	gcc -o 9113file -L/usr/lib -lpci_dask -lpthread 9113file.o conio.o

9113file.o: 9113file.c
	gcc -c $(CFLAGS) -I../../../include 9113file.c

conio.o: ../../conio/conio.c
	gcc -c -I../../../include ../../conio/conio.c

clean:
	rm -f 9113file 9113file.o conio.o 9113dat.txt
