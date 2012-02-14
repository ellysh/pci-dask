/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2010/01/07                                                       */
/*                                                                            */
/* This sample performs continuous AO DMA update with infinite delay trigger  */
/* and wave repeats.                                                          */
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
#define TIMEBASE 80000000.0
#define UPDATEINTRV 10000
#define PI 3.1415926

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 ConfigCtrl = P922x_AO_CONVSRC_INT;
    U16 TrigCtrl = P922x_AO_TRGMOD_DELAY|P922x_AO_TRGSRC_GPI0|P922x_AO_EnReTigger|P922x_AO_EnDelay2;
    U32 ReTrgCnt = 0; //0 means infinite trigger if retrigger is enabled
    U32 DLY1Cnt = 80000000; //Trigger delay count; Delay time = DLY1Cnt/TIMEBASE sec.
    U32 DLY2Cnt = 40000000; //Wave separation count; Delay time = DLY2Cnt/TIMEBASE sec.
    BOOLEAN AutoResetBuf = 1; //Auto reset buffer descriptor built in kernel
    U32 W_Buffer[WRITECOUNT]; //Data buffer to be written
    U32 WriteCount = WRITECOUNT; //Wrtie count per one wave/one trigger
    U16 BufferId;
    U16 Channel = 0; //AO channel to be written
    U32 Iterations = 4; //Wave iterations per one trigger
    U32 CHUI = UPDATEINTRV;
    U16 definite = 1;
    U32 vi;
    U32 AccessCnt;
    BOOLEAN Stopped;

    for(vi=0; vi<WriteCount; vi++){
        W_Buffer[vi] = (U16)((sin((double)vi/WriteCount*2*PI)*0x7fff)+0x8000);
    }

    printf("This sample performs continuous AO update to Channel %d \n", Channel);
    printf("at %6.2f update rate with infinit trigger.\n", (F32)TIMEBASE/UPDATEINTRV);
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    /*Open and initialize device*/
    card = Register_Card(PCI_9223, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    /*Configure AO*/
    err = AO_9223_Config(card, ConfigCtrl, TrigCtrl, ReTrgCnt, DLY1Cnt, DLY2Cnt, AutoResetBuf);
    if(err<0){
        printf("AO_9223_Config Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Disable double-buffer mode*/
    err = AO_AsyncDblBufferMode(card, 0);
    if(err<0){
        printf("AO_AsyncDblBufferMode Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Setup memory descriptor for DMA transfer*/
    err = AO_ContBufferSetup(card, W_Buffer, WriteCount, &BufferId);
    if(err<0){
        printf("AO_ContBufferSetup Error: %d\n", err);
        AO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    /*Start AO operation*/
    err = AO_ContWriteChannel(card, Channel, BufferId, WriteCount, Iterations, CHUI, definite, ASYNCH_OP);
    if(err<0){
        printf("AO_ContWriteChannel Error: %d\n", err);
        AO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }
    printf("\n\nWait infinite digital trigger from GPI0...\n");
    printf("You can press Enter to stop AO update...\n");
    do{
        err = AO_AsyncCheck(card, &Stopped, &AccessCnt);
        if(err<0){
            printf("AO_AsyncCheck Error: %d\n", err);
            AO_ContBufferReset(card);
            Release_Card(card);
            exit(1);
        }
    }while(!kbhit());

    AO_AsyncClear(card, &AccessCnt, 0);
    printf("\nAO had been stopped...\n");

    if(!AutoResetBuf)
        AO_ContBufferReset(card);
    Release_Card(card);

    printf("Press any key to exit...\n");
    getch();
    return 0;
}
