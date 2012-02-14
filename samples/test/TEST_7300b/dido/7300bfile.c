#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dask.h"
#include "conio.h"

U16 DoBuf[20000];
U16 DiBuf[10];

U32 OutCount = 20000;
U32 InCount = 10;

int main()
{
    I16 card1, card2, err;
	int i;
	BOOLEAN bStopped;
	U32 count;

    if((card1=Register_Card(PCI_7300A_RevB, 0))<0){
        printf("Register_Card error=%d\n", card1);
        exit(1);
    }
    if((card2=Register_Card(PCI_7300A_RevB, 1))<0){
        printf("Register_Card error=%d\n", card2);
        exit(1);
    }	

    for(i=0; i<OutCount; i++)
        DoBuf[i] = (U16)i;
    for(i=0; i<InCount; i++)
        DiBuf[i] = 0;

    err = DO_7300B_Config(card1, 16, TRIG_CLK_20MHZ, P7300_WAIT_NO, P7300_TERM_ON, 0, 0x40004000);
    if(err!=0){
       printf("DO_7300B_Config error=%d\n", err);
       exit(1);
    }

    err = DI_7300B_Config(card2, 16, TRIG_CLK_10MHZ, P7300_WAIT_NO, P7300_TERM_ON, 0, 1, 1);
    if(err!=0){
       printf("DI_7300B_Config error=%d\n", err);
       exit(1);
    }

    err = DO_ContWritePort(card1, 0, DoBuf, OutCount, 1, 1/*ignored*/, ASYNCH_OP);
    if(err!=0){
       printf("DO_ContWritePort error=%d", err);
	   exit(1);
    }

    err = DI_ContReadPort(card2, 0, DiBuf, InCount, 1/*ignored*/, ASYNCH_OP);
    if (err!=0) {
       printf("DI_ContReadPort error=%d", err);
       exit(1);
    }

    do{
         DO_AsyncCheck(card1, &bStopped, &count);
    }while (!bStopped);

    do {
         DI_AsyncCheck(card2, &bStopped, &count);
    } while (!bStopped);

	for(i=0; i<InCount; i++)
		printf("%d\n", DiBuf[i]);

	DO_AsyncClear(card1, &count);
    DI_AsyncClear(card2, &count);
 	
    Release_Card(card1);
    Release_Card(card2);

	return 0;
}
