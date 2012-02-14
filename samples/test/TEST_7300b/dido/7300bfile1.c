#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dask.h"
#include "conio.h"

#define OutCount 1000
#define InCount 1000

U16 DoBuf[OutCount];
U16 DiBuf[InCount];


int main()
{
    I16 card1, card2, err;
	int i;
	BOOLEAN bStopped = 0;
	U32 count1, count2;

    if((card1=Register_Card(PCI_7300A_RevB, 0))<0){
        printf("Register_Card error=%d\n", card1);
        exit(1);
    }
    if((card2=Register_Card(PCI_7300A_RevB, 1))<0){
        printf("Register_Card error=%d\n", card2);
        exit(1);
    }

    for(i=0; i<OutCount; i++)
        DoBuf[i] = i&0x7fff;
    for(i=0; i<InCount; i++)
        DiBuf[i] = 0;

    err = DO_7300B_Config(card1, 16, /*TRIG_CLK_20MHZ*/TRIG_INT_PACER, P7300_WAIT_NO, P7300_TERM_ON, 0, 0x40004000);
    if(err!=0){
       printf("DO_7300B_Config error=%d\n", err);
       exit(1);
    }

    err = DI_7300B_Config(card2, 16, /*TRIG_CLK_10MHZ*/TRIG_INT_PACER, P7300_WAIT_NO, P7300_TERM_ON, 0, 1, 1);
    if(err!=0){
       printf("DI_7300B_Config error=%d\n", err);
       exit(1);
    }

    err = DO_ContWritePort(card1, 0, DoBuf, OutCount, 1, OutCount/*1*//*ignored*/, ASYNCH_OP);
    if(err!=0){
       printf("DO_ContWritePort error=%d", err);
	   exit(1);
    }

    err = DI_ContReadPort(card2, 0, DiBuf, InCount, InCount/*1*//*ignored*/, ASYNCH_OP);
    if (err!=0) {
       printf("DI_ContReadPort error=%d", err);
       exit(1);
    }

    do{
         DO_AsyncCheck(card1, &bStopped, &count1);
    }while (!bStopped);
	printf("bStopped: %d, count1: %d\n", bStopped, count1);

	bStopped = 0;
    do {
         DI_AsyncCheck(card2, &bStopped, &count2);
    } while (!bStopped);
	printf("bStopped: %d, count2: %d\n", bStopped, count2);

	for(i=0; i<count2; i++)
		printf("%#x\t", DiBuf[i]);

	DO_AsyncClear(card1, &count1);
    DI_AsyncClear(card2, &count2);
 	
    Release_Card(card1);
    Release_Card(card2);

	return 0;
}
