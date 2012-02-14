CFLAGS=-Wall -Wstrict-prototypes -O2

calibration: calibration.o conio.o
	gcc -o calibration -L/usr/lib -lpci_dask calibration.o conio.o

calibration.o: calibration.c
	gcc -c $(CFLAGS) -I../../../include calibration.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) -I../../../include ../../conio/conio.c
	
clean:
	rm -f calibration *.o 
