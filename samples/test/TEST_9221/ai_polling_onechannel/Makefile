CFLAGS=-Wall -Wstrict-prototypes -O2

9221ai: 9221ai.o conio.o
	gcc -o 9221ai -L/usr/lib -lpci_dask 9221ai.o conio.o

9221ai.o: 9221ai.c
	gcc -c $(CFLAGS) -I../../../../include 9221ai.c

conio.o: ../../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../../include ../../../conio/conio.c
	
clean:
	rm -f 9221ai *.o 
