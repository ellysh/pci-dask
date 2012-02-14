/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2010/01/07                                                       */
/*                                                                            */
/* This sample updates AO voltage with external conversion source.            */
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

#define WRITECOUNT 8000
#define PI 3.1415926

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 ConfigCtrl = P922x_AO_CONVSRC_GPI0;
    U16 TrigCtrl = 0; //Dose not support any trigger mode/source in external conversion source mode
    U32 ReTrgCnt = 0;
    U32 DLY1Cnt = 0;
    U32 DLY2Cnt = 0;
    BOOLEAN AutoResetBuf = 1;
    U32 W_Buffer[WRITECOUNT];
    U32 WriteCount = WRITECOUNT;
    U16 BufferId;
    U16 Channel = 0;
    U32 Iterations = 1; //Only support 1 or infinite iterations in external conversion source mode
    U32 CHUI = 0; //Ignore in external conversion source mode
    U16 definite = 1; //Only support definite mode in external conversion source mode
    U16 SyncMode = ASYNCH_OP; //Only support ASYNC. mode in external conversion source mode
    U32 vi;
    U32 AccessCnt;
    BOOLEAN Stopped;

    for(vi=0; vi<WriteCount; vi++){
        W_Buffer[vi] = (U16)((sin((double)vi/WriteCount*2*PI)*0x7fff)+0x8000);
    }

    printf("This sample updates voltage to AO Channel 0 with external conversion source.\n");
    printf("Updates one AO value while GPI0 is asserted.\n");

    printf("Card Number? ");
    scanf(" %hd", &card_num);

    /*Open and initialize device*/
    card = Register_Card(PCI_9222, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    /*Configure 9222 AO*/
    err = AO_9222_Config(card, ConfigCtrl, TrigCtrl, ReTrgCnt, DLY1Cnt, DLY2Cnt, AutoResetBuf);
    if(err<0){
        printf("AO_9222_Config Error: %d\n", err);
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
        printf("AO_ContBufferSetup Error: %d\n", err);
        AO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    /*Start AO*/
    printf("\nPress any key to start AO to wait external concersion source from GPI0.\n");
    printf("And you can press Enter to stop the operation...\n");
    getch();
    err = AO_ContWriteChannel(card, Channel, BufferId, WriteCount, Iterations, CHUI, definite, SyncMode);
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
        printf("\nAO Update Done...\n");
    else
        printf("\nAO will be stopped manually...\n");
    AO_AsyncClear(card, &AccessCnt, 0);

    if(!AutoResetBuf)
        AO_ContBufferReset(card);
    Release_Card(card);
    printf("Press any key to exit...\n");
    getch();
    return 0;
}
