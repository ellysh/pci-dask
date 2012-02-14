CFLAGS=-Wall -Wstrict-prototypes -O2

all: 7300bfile.o conio.o 7300bfile1.o
	gcc -o 7300bfile -L/usr/lib -lpci_dask -lm 7300bfile.o conio.o
	gcc -o 7300bfile1 -L/usr/lib -lpci_dask -lm 7300bfile1.o conio.o

#7300bfile1: 7300bfile1.o conio.o
#	gcc -o 7300bfile1 -L/usr/lib -lpci_dask -lm 7300bfile1.o conio.o

7300bfile.o: 7300bfile.c
	gcc -c $(CFLAGS) -I../../../../include 7300bfile.c

7300bfile1.o: 7300bfile1.c
	gcc -c $(CFLAGS) -I../../../../include 7300bfile1.c

conio.o: ../../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../../include ../../../conio/conio.c

clean:
	rm -f 7300bfile 7300bfile.o 7300bfile1 7300bfile1.o conio.o
