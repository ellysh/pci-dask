/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2010/01/07                                                       */
/*                                                                            */
/* This sample Updates AO voltage with DMA mode.                              */
/* There are 2 AO channels in PCI-9223. You can use polling mode or DMA to    */
/* to update data to specified channels.                                      */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "dask.h"

#define WRITECOUNT 8000
#define TIMEBASE 80000000
#define UPDATEINTRV 10000
#define PI 3.1415926
#define AO_SYNCTIMEOUT 5000

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 ConfigCtrl = P922x_AO_CONVSRC_INT;
    U16 TrigCtrl = P922x_AO_TRGMOD_POST|P922x_AO_TRGSRC_SOFT;
    U32 ReTrgCnt = 0; //Ignored in non-retrigger mode
    U32 DLY1Cnt = 0; //Ignored in non-delay trigger mode
    U32 DLY2Cnt = 0; //Ignored in non-delay trigger mode
    BOOLEAN AutoResetBuf = 1;
    U32 W_Buffer[WRITECOUNT];
    U32 Patten[WRITECOUNT];
    U32 WriteCount = WRITECOUNT;
    U16 BufferId;
    U16 Channel = 0;
    U32 Iterations = 1;
    U32 CHUI = UPDATEINTRV;
    U16 definite = 1;
    U32 vi;
    U32 TimeOut_ms = AO_SYNCTIMEOUT;

    for(vi=0; vi<WriteCount; vi++){
        Patten[vi] = (U16)((sin((double)vi/WriteCount*2*PI)*0x7fff)+0x8000);
    }
    memcpy(W_Buffer, Patten, WriteCount*sizeof(U32));

    printf("This sample updates voltage to AO Channel 0\n");
    printf("at %6.2f update rate.\n", (float)TIMEBASE/UPDATEINTRV);
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    /*Open and initialize device*/
    card = Register_Card(PCI_9223, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    /*Configure 9223 AO*/
    err = AO_9223_Config(card, ConfigCtrl, TrigCtrl, ReTrgCnt, DLY1Cnt, DLY2Cnt, AutoResetBuf);
    if(err<0){
        printf("AO_9223_Config Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Set Timeout Period for AO Sync Mode*/
    err = AO_SetTimeOut(card, TimeOut_ms);
    if(err<0){
        printf("AO_SetTimeOut Error: %d\n", err);
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

    /*Setup buffer*/
    err = AO_ContBufferSetup(card, W_Buffer, WriteCount, &BufferId);
    if(err<0){
        printf("AO_ContBufferSetup0 Error: %d\n", err);
        AO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    /*Start AO*/
    err = AO_ContWriteChannel(card, Channel, BufferId, WriteCount, Iterations, CHUI, definite, SYNCH_OP);
    if(err<0){
        printf("AO_ContWriteChannel Error: %d\n", err);
        AO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    printf("\n\nAO Update Done...\n");

    if(!AutoResetBuf)
        AO_ContBufferReset(card);
    Release_Card(card);

    printf("Press any key to exit...\n");
    getch();
    return 0;
}
