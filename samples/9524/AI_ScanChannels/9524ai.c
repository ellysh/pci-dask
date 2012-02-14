/*-----------------------------------------------------------------------------*/
/* Date:    2009/08/19                                                         */
/* Company: AdLINK Technology Inc.                                             */
/*                                                                             */
/* This sample scans AI channels to acquire analog voltage.                    */
/* PCI-9524 has two AI group, load cell group and general purpose group.       */
/* You must configure the AI group by calling AI_9524_Config function.         */
/* AI_9524_PollConfig function should be called to start ADC while the         */
/* AI transfer mode is set to polling mode.                                    */
/* Some advanced setting related DSP is required while load cell group is      */
/* performing.                                                                 */
/*-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include "dask.h"

int main(int argc, char **argv)
{
    U16 card_num;
    I16 card, err;
    U16 Group;
    U16 XMode = P9524_AI_XFER_POLL;
    U16 ConfigCtrl;
    U16 PollChannel = P9524_AI_POLL_ALLCHANNELS;
    U16 PollRange;
    U16 PollSpeed = P9524_ADC_60_SPS;
    U32 VBuffer[4];
    U16 i;
    F64 voltage[4];
    F64 ActualRate;

    printf("This sample performs AI Channel Scan operation.\n");
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    /*Open and initialize device*/
    card = Register_Card(PCI_9524, card_num);
    if(card<0){
        printf("Register Card Failed: %d\n", card);
        exit(1);
    }

    printf("Load Cell Channel[0] or General Purpose Channel[1]? [0]or[1] ");
    scanf(" %hd", &Group);
    if(Group==0){
        /*Load Cell AI*/
        ConfigCtrl = P9524_VEX_Range_10V|P9524_VEX_Sence_Remote|P9524_AI_AZMode;
        PollRange = 0;
    }
    else if(Group==1){
        /*General Purpose AI*/
        ConfigCtrl = 0;
        PollRange = AD_B_10_V;
    }
    else{
        printf("invalid group\n");
        Release_Card(card);
        exit(1);
    }

    if(Group==0){
        /*Load Cell AI need extra DSP configuration*/
        for(i=0; i<4; i++){
            err = AI_9524_SetDSP(card, i, P9524_SPIKE_REJ_ENABLE, 2, 16);
            if(err<0){
                printf("AI_9524_SetDSP Error: %d\n", err);
                Release_Card(card);
                exit(1);
            }
        }
    }

    /*Configure AI*/
    err = AI_9524_Config(card, Group, XMode, ConfigCtrl, 0, 0);
    if(err<0){
        printf("AI_9524_Config Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Configure AI polling mode*/
    err = AI_9524_PollConfig(card, Group, PollChannel, PollRange, PollSpeed);
    if(err<0){
        printf("AI_9524_PollConfig Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    err = GetActualRate_9524(card, Group, PollSpeed, &ActualRate);
    if(err<0)
        printf("GetActualRate_9524 Error: %d\n", err);
    printf("\nADC started at speed: %7.2lf SPS\n", ActualRate);
    printf("Press any key to start read AI channel...\n");
    getch();

    do{
        usleep(100000);
        err = AI_ScanReadChannels32(card, Group, PollRange, VBuffer);
        if(err<0){
            printf("AI_ScanReadChannels32 Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        //AI_VoltScale32(card, PollRange, Value, &voltage);
        AI_ContVScale(card, PollRange, VBuffer, voltage, 4);
        clrscr();
        printf("/*---------------------------------*/\n");
        for(i=0; i<4; i++){
            printf("Group %d - Channel %d\n", Group, i);
            printf("raw data: 0x%x\n", VBuffer[i]);
            printf("scaled voltage: %lf\n\n", voltage[i]);
        }
        printf("            press Enter to exit...\n");
        printf("/*---------------------------------*/\n");
    }while(!kbhit());

    printf("Press any key to exit...\n");
    getch();
    Release_Card(card);
    return 0;
}
