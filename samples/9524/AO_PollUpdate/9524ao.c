/*----------------------------------------------------------------------------*/
/* Date: 2009/08/19                                                           */
/* Company: ADLINK Technology Inc.                                            */
/*                                                                            */
/* PCI-9524 has two AO port and the range is from -10V to 10V.                */
/* This sample updates the indicated voltage to the indicated AO port.        */
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
    U16 Channel;
    U8 fStop;
    F64 OutValue;

    printf("This sample updates voltage value to AO port.\n");
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    card = Register_Card(PCI_9524, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    do{
        clrscr();

        printf("Update Channel? (0)or(1): ");
        scanf(" %hd", &Channel);
        if(Channel>1){
            printf("Invalid Channel Number\n");
            exit(1);
        }

        printf("Update Voltage? [-10 ~ 10]: ");
        scanf(" %lf", &OutValue);
        /*AO range: -10V ~ 10V*/
        OutValue = (OutValue>10.0)? 10:OutValue;
        OutValue = (OutValue<-10.0)? -10:OutValue;

        /*Update Voltage*/
        err = AO_VWriteChannel(card, Channel, OutValue);
        if(err<0){
            printf("AO_WriteChannel Error: %d\n", err);
        }

        printf("\npress 'q' or 'Q' to stop or others to continue...\n");
        fStop = getch();
    }while((fStop!='q')&&(fStop!='Q'));

    Release_Card(card);
    return 0;
}
