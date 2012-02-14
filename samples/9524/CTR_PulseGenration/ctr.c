/*----------------------------------------------------------------------------*/
/* Date: 2010/01/08                                                           */
/* Company: ADLINK Technology Inc.                                            */
/*                                                                            */
/* PCI-9524 has three Timer/Counter to generate CW/CCW, CLK/DIR pulse.        */
/* (PulseValue+PulseLength)/40000000 is the period of the generated pulse.    */
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
    U16 Mode = P9524_PulseGen_CCW;
    U32 PulseValue = 2000000;
    U32 PulseLength = 2000000;
    U32 PulseGenNum = 1000;

    printf("This sample generates %d CCW pulse in %f HZ.\n", PulseGenNum, 40000000.0/(PulseValue+PulseLength));
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    card = Register_Card(PCI_9524, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    printf("GPTC Number? [0~2] ");
    scanf(" %hd", &GCtr);
    if(GCtr>2){
        printf("Invalid Counter. Force to set to GPTC0.\n\n");
        GCtr = 0;
    }

    /*Clear*/
    GPTC_Clear(card, GCtr);

    /*Setup*/
    err = GPTC_Setup(card, GCtr, Mode, 0, 0, PulseValue, PulseLength);
    if(err<0){
        printf("GPTC_Setup Error: %d\n", err);
        GPTC_Clear(card, GCtr);
        Release_Card(card);
        exit(1);
    }

    /*advanced configuration for pulse generation*/
    err = GPTC_9524_PG_Config(card, GCtr, PulseGenNum);
    if(err<0){
        printf("GPTC_9524_PG_Config Error: %d\n", err);
        GPTC_Clear(card, GCtr);
        Release_Card(card);
        exit(1);
    }

    printf("Press any key to start\n");
    getch();
    /*Enable*/
    err = GPTC_Control(card, GCtr, P9524_CTR_Enable, 1);
    if(err<0){
        printf("GPTC_Control Error: %d\n", err);
        GPTC_Clear(card, GCtr);
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
        printf("/*----------------------------------------*/\n");
        printf("Counter counts down while generating pulse.\n");
        printf("Counter Value: %d\n\n", Value);
        printf("                      Press Enter to stop...\n");
        printf("/*----------------------------------------*/\n");
        usleep(1000);
    }while(!kbhit());

    GPTC_Control(card, GCtr, P9524_CTR_Enable, 0);
    GPTC_Clear(card, GCtr);

    printf("Press any key to exit\n");
    getch();
    Release_Card(card);
    return 0;
}
