/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2010/01/06                                                       */
/*                                                                            */
/* This sample performs X4 AB phase encode.                                   */
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
    U16 GCtr;
    U16 Mode = x4_AB_Phase_Encoder;
    U16 SrcCtrl = 0; //Not used in encoder
    U16 PolCtrl = 0; //Not used in encoder
    U32 LReg1_Val = 0; //Not used in encoder
    U32 LReg2_Val = 0; //Not used in encoder
    U32 RD_Value;
    int i = 0;

    printf("This sample performs x4 AB phase mode.\n");
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    card = Register_Card(PCI_9222, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    printf("Encoder Number? ");
    scanf(" %hd", &GCtr);
    GCtr = (GCtr==1)? P922x_ENCODER1:P922x_ENCODER0;

    /*Clear Encoder*/
    err = GPTC_Clear(card, GCtr);
    if(err<0){
        printf("GPTC_Clear Error: %d\n", err);
        goto ERR_RET;
    }

    /*Setup Encoder*/
    err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, LReg1_Val, LReg2_Val);
    if(err<0){
        printf("GPTC_Setup Error: %d\n", err);
        goto ERR_RET;
    }

    /*Enable Encoder*/
    printf("Press any key to enable counter...\n");
    getch();
    err = GPTC_Control(card, GCtr, IntENABLE, 1);
    if(err<0){
        printf("GPTC_Control(IntENABLE) Error: %d\n", err);
        goto ERR_RET;
    }

    do{
        /*Read Encoder*/
        err = GPTC_Read(card, GCtr, &RD_Value);
        if(err<0){
            printf("GPTC_Read Error: %d\n", err);
            goto ERR_RET2;
        }
        clrscr();
        printf("/*---------------------------------*/\n");
        printf(" Encoder Number: %d\n", GCtr-4);
        printf(" Read Count: %d\n\n", RD_Value);
        printf("      You can press Enter to stop...\n");
        printf("/*---------------------------------*/\n");
        usleep(1000);
    }while(!kbhit());

ERR_RET2:
    GPTC_Control(card, GCtr, IntENABLE, 0);
ERR_RET:
    GPTC_Clear(card, GCtr);
    Release_Card(card);

    printf("\nPress any key to exit...\n");
    getch();
    return 0;
}
