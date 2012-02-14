/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2010/01/07                                                       */
/*                                                                            */
/* This sample Updates AO voltage with polling mode.                          */
/* There are 2 AO channels in PCI-9222. You can use polling mode or DMA to    */
/* to update data to specified channels.                                      */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "dask.h"

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 Channel;
    F64 Voltage;
    U8 fStop;

    printf("This sample updates voltage to AO Channel.\n");
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    card = Register_Card(PCI_9222, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    do{
        clrscr();
        printf("AO Channel Number to be update: [0 or 1] ");
        scanf(" %hd", &Channel);
        if(Channel>1){
            printf("Invalid Channel Number... Set to Channel 0\n");
            Channel = 0;
        }
        printf("AO voltage to be updated: [-10 ~ 10] ");
        scanf(" %lf", &Voltage);
        if(Voltage>10){
            printf("Out of range, forcedly ouput 10V\n");
            Voltage = 10;
        }
        if(Voltage<-10){
            printf("Out of range, forcedly ouput -10V\n");
            Voltage = -10;
        }

        err = AO_VWriteChannel(card, Channel, Voltage);
        if(err<0){
            printf("AO_VWriteChannel Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        printf("\n(C)ontinue? ");
        scanf(" %c", &fStop);
    }while((fStop=='C')||(fStop=='c'));

    Release_Card(card);
    return 0;
}
