/*----------------------------------------------------------------------------*/
/* Date: 2009/08/19                                                           */
/* Company: ADLINK Technology Inc.                                            */
/*                                                                            */
/* PCI-9524 has two AO ports and the range is from -10V to 10V.               */
/* This sample updates the two AO ports simultaneously.                       */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

int main(int argc, char **argv)
{
    U16 card_num;
    I16 card;
    I16 err;
    U16 Group = P9524_AO_CH0_1;
    U8 fStop;
    F64 VBuffer[2];

    printf("This sample updates voltage values to the two AO ports simultaneously.\n");
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    card = Register_Card(PCI_9524, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    do{
        clrscr();

        printf("AO Channel 0 Update Voltage? [-10 ~ 10]: ");
        scanf(" %lf", &VBuffer[0]);
        VBuffer[0] = (VBuffer[0]>10.0)? 10:VBuffer[0];
        VBuffer[0] = (VBuffer[0]<-10.0)? -10:VBuffer[0];

        printf("AO Channel 1 Update Voltage? [-10 ~ 10]: ");
        scanf(" %lf", &VBuffer[1]);
        VBuffer[1] = (VBuffer[1]>10.0)? 10:VBuffer[1];
        VBuffer[1] = (VBuffer[1]<-10.0)? -10:VBuffer[1];

        /*Simultaneously Update Voltage*/
        err = AO_SimuVWriteChannel(card, Group, VBuffer);
        if(err<0){
            printf("AO_SimuVWriteChannel Error: %d\n", err);
        }

        printf("\npress 'q' or 'Q' to stop or others to continue...\n");
        fStop = getch();
    }while((fStop!='q')&&(fStop!='Q'));

    Release_Card(card);
    return 0;
}
