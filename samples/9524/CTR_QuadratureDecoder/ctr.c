/*----------------------------------------------------------------------------*/
/* Date: 2010/01/08                                                           */
/* Company: ADLINK Technology Inc.                                            */
/*                                                                            */
/* PCI-9524 has three Quadrature Decoder to decodes x4_AB_Phase Pulse.        */
/* This sample decodes the x4_AB_Pulse and shows the decoded step.            */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <time.h>
#include "dask.h"


int main(int argc, char **argv)
{
    U16 card_num;
    I16 card;
    I16 err;
    U16 GCtr;
    U32 Value;
    U16 Mode = P9524_x4_AB_Phase_Decoder;

    printf("This sample Decodes X4_AB_Phase Pulse by Decoder.\n");
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    card = Register_Card(PCI_9524, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    printf("Decoder Number? [0~2] ");
    scanf(" %hd", &GCtr);
    if(GCtr==0)
        GCtr = P9524_CTR_QD0;
    else if(GCtr==1)
        GCtr = P9524_CTR_QD1;
    else if(GCtr==2)
        GCtr = P9524_CTR_QD2;
    else{
        printf("Invalid Decoder. Force to Set to Decoder 0\n\n");
        GCtr = P9524_CTR_QD0;
    }

    err = GPTC_Clear(card, GCtr);
    if(err<0){
        printf("GPTC_Clear Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    err = GPTC_Setup(card, GCtr, Mode, 0, 0, 0, 0);
    if(err<0){
        printf("GPTC_Setup Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    printf("Press any key to start the specified Quadrature Decoder\n");
    getch();

    err = GPTC_Control(card, GCtr, P9524_CTR_Enable, 1);
    if(err<0){
        printf("GPTC_Control Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    do{
        err = GPTC_Read(card, GCtr, &Value);
        if(err<0){
            printf("GPTC_Read Error: %d\n", err);
            break;
        }
        clrscr();
        printf("/*------------------------------*/\n");
        printf("    Decoder: %d\n", GCtr-3);
        printf("    Raw Data: 0x%x\n", Value);
        printf("    2's Complement Value: %d\n\n", (int)(Value*256)/256);
        printf("           Press Enter to stop...\n");
        printf("/*------------------------------*/\n");
        usleep(1000);
    }while(!kbhit());

    err = GPTC_Control(card, GCtr, P9524_CTR_Enable, 0);
    if(err<0){
        printf("GPTC_Control Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    printf("Press any key to exit\n");
    getch();

    err = GPTC_Clear(
        card,
        GCtr
    );
    if(err<0){
        printf("GPTC_Clear Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    Release_Card(card);
    return 0;
}
