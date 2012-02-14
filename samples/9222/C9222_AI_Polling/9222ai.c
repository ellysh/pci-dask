/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2010/01/07                                                       */
/*                                                                            */
/* This sample performs AI acquisition with polling mode.                     */
/* There are 16(Single Ended Mode)/8(Differetial Mode) AI channels in         */
/* PCI-9222. You can use polling mode or DMA to acquired data from specified  */
/* channels.                                                                  */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <string.h>
#include "dask.h"


int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;

    U16 Channel = 0;
    U16 AdRange = AD_B_10_V;
    U16 Value;
    F64 Voltage;

    printf("This sample acquires analog voltage from AI Channel 0 with polling mode.\n");
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    card = Register_Card(PCI_9222, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    do{
        err = AI_ReadChannel(card, Channel, AdRange, &Value);
        if(err<0){
            printf("AI_ReadChannel Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        clrscr();
        printf("------------------------------------\n");
        AI_VoltScale(card, AdRange, Value, &Voltage);
        printf("  Channel %d :\n", Channel);
        printf("  Acquired Raw Data : 0x%x\n", Value);
        printf("  Scaled Voltage : %8.6lf\n", Voltage);
        printf("\n            Press Enter to exit...\n");
        printf("------------------------------------\n");
        usleep(10000);
    }while(!kbhit());

    Release_Card(card);
    return 0;
}
