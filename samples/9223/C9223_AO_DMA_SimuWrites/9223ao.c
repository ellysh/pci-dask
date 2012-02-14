/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2010/01/07                                                       */
/*                                                                            */
/* This sample simultaneously updates AO voltage to AO two channels with DMA. */
/* For example, AO_Buffer[0] and AO_Buffer[1] are simultaneously updating to  */
/* AO Channel 0 and 1, then, AO_Buffer[2] and AO_Buffer[3] are simultaneously */
/* updating to AO Channel 0 and 1, ..., etc. AO_ContBufferCompose() can be    */
/* used to compose to an arrange buffer for multi-channels of continuous AO.  */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "dask.h"

#define WRITECHNCNT 2
#define WRITECOUNT 8000
#define BUFCOUNT WRITECOUNT*WRITECHNCNT
#define TIMEBASE 80000000
#define UPDATEINTRV 10000
#define PI 3.1415926

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 ConfigCtrl = P922x_AO_CONVSRC_INT;
    U16 TrigCtrl = P922x_AO_TRGMOD_POST|P922x_AO_TRGSRC_SOFT;
    U32 ReTrgCnt = 0; //Ignored in non-retrigger mode
    U32 DLY1Cnt = 0; //Ignored in non-delay trigger mode
    U32 DLY2Cnt = 0; //Ignored in non-delay2 mode
    BOOLEAN AutoResetBuf = 1; //Auto reset buffer descriptor built in kernel
    U32 W_Buffer[BUFCOUNT]; //Data buffer to be written
    U32 Patten0[WRITECOUNT];
    U32 Patten1[WRITECOUNT];
    U32 WriteCount = WRITECOUNT; //AO write count
    U16 BufferId;
    U16 NumChans = WRITECHNCNT; //Numbers of channel to be written
    U16 Chans[WRITECHNCNT] = {0, 1}; //AO channels
    U32 Iterations = 1; //write once
    U32 CHUI = UPDATEINTRV;
    U16 definite = 1;
    U32 vi;
    U32 AccessCnt;
    BOOLEAN Stopped;
    U32 BufCount = BUFCOUNT;

    printf("This sample simultaneously updates voltage to AO Channels \n");
    printf("at %6.2f update rate.\n", (float)TIMEBASE/UPDATEINTRV);
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    /*Open and initialize device*/
    card = Register_Card(PCI_9223, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    for(vi=0; vi<WriteCount; vi++){
        Patten0[vi] = (U16)((sin((double)vi/WriteCount*2*PI)*0x7fff)+0x8000);
        Patten1[vi] = (U16)((float)(vi*65535)/(float)WriteCount);
    }
    /*Compose the two buffers*/
    err = AO_ContBufferCompose(card, NumChans, Chans[0], WriteCount, W_Buffer, Patten0);
    if(err<0){
        printf("AO_ContBufferCompose0 Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    err = AO_ContBufferCompose(card, NumChans, Chans[1], WriteCount, W_Buffer, Patten1);
    if(err<0){
        printf("AO_ContBufferCompose1 Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Configure 9223 AO*/
    err = AO_9223_Config(card, ConfigCtrl, TrigCtrl, ReTrgCnt, DLY1Cnt, DLY2Cnt, AutoResetBuf);
    if(err<0){
        printf("AO_9223_Config Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Disable Double Buffer Mode*/
    err = AO_AsyncDblBufferMode(card, 0);
    if(err<0){
        printf("AO_AsyncDblBufferMode Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Buffer Setup for DMA transfer*/
    err = AO_ContBufferSetup(card, W_Buffer, BufCount, &BufferId);
    if(err<0){
        printf("AO_ContBufferSetup0 Error: %d\n", err);
        AO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    /*Start AO update*/
    err = AO_ContWriteMultiChannels(card, NumChans, Chans, BufferId, WriteCount, Iterations, CHUI, definite, ASYNCH_OP);
    if(err<0){
        printf("AO_ContWriteChannel Error: %d\n", err);
        AO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }
    do{
        err = AO_AsyncCheck(card, &Stopped, &AccessCnt);
        if(err<0){
            printf("AO_AsyncCheck Error: %d\n", err);
            AO_ContBufferReset(card);
            Release_Card(card);
            exit(1);
        }
    }while((!kbhit())&&(!Stopped));

    if(Stopped)
        printf("\n\nAO Update Done...\n");
    else
        printf("\n\nAO will be stopped...\n");

    AO_AsyncClear(card, &AccessCnt, 0);
    printf("%d Samples had been written...\n", AccessCnt);

    if(!AutoResetBuf)
        AO_ContBufferReset(card);
    Release_Card(card);

    printf("Press any key to exit...\n");
    getch();
    return 0;
}
