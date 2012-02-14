# Makefile for sample programs

CC=gcc
CFLAGS := -Wall -Wstrict-prototypes -O2 -I../../../include

6202_PWM: 6202_PWM.o conio.o
	gcc -o 6202_PWM -lpci_dask 6202_PWM.o  conio.o

6202_PWM.o: 6202_PWM.c
	gcc -c $(CFLAGS) 6202_PWM.c

conio.o: ../../conio/conio.c
	gcc -c $(CFLAGS) ../../conio/conio.c

clean:
	rm -f 6202_PWM *.o
