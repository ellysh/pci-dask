/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2010/01/07                                                       */
/*                                                                            */
/* This sample performs continuous DO operation with infinite trigger.        */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <string.h>
#include "dask.h"

#define WRITECOUNT 4000
#define TIMEBASE 80000000
#define SAMPRATE 4000

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;

    U16 ConfigCtrl = P922x_DO_CONVSRC_INT;
    U16 TrigCtrl = P922x_DO_TRGMOD_POST|P922x_DO_TRGSRC_GPI0|P922x_DO_EnReTigger;
    U32 ReTrgCnt = 0; //0 means infinite trigger in retrigger mode
    U32 DLY1Cnt = 0; //Ignored in non-delay trigger mode
    U32 DLY2Cnt = 0; //No used in 922x do
    BOOLEAN AutoResetBuf = 1; //Auto reset buffer descriptors built in driver
    U32 W_Buffer[WRITECOUNT]; //Data buffer to be written
    U32 WriteCount = WRITECOUNT; //Output count
    U16 BufferId;
    U16 Port = 0; //Only one DO port
    U16 Iterations = 1; //922x has no DO repeats
    F64 SampleRate = SAMPRATE;
    BOOLEAN Stopped;
    U32 AccessCnt = 0;
    U32 vi;

    for(vi=0; vi<WriteCount; vi++){
        W_Buffer[vi] = (U16)(((F32)vi/WriteCount)*65535);
    }

    printf("This sample performs continuous DO with infinite trigger \n");
    printf("at %6.2f update rate.\n", SampleRate);
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    card = Register_Card(PCI_9223, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    err = DO_9223_Config(card, ConfigCtrl, TrigCtrl, ReTrgCnt, DLY1Cnt, DLY2Cnt, AutoResetBuf);
    if(err<0){
        printf("DO_9223_Config Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    err = DO_ContBufferSetup(card, W_Buffer, WriteCount, &BufferId);
    if(err<0){
        printf("DO_ContBufferSetup Error: %d\n", err);
        DO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    err = DO_ContWritePort(card, Port, &BufferId, WriteCount, Iterations, SampleRate, ASYNCH_OP);
    if(err<0){
        printf("DO_ContWritePort Error: %d\n", err);
        DO_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }
    printf("\nDO Update is started... Wait infinite trigger from GPI0...\n");
    printf("You can press Enter to stop it...\n");
    do{
        err = DO_AsyncCheck(card, &Stopped, &AccessCnt);
        if(err<0){
            printf("DO_AsyncCheck Error: %d\n", err);
            DO_ContBufferReset(card);
            DO_AsyncClear(card, &AccessCnt);
            Release_Card(card);
            exit(1);
        }
    }while(!kbhit());
    DO_AsyncClear(card, &AccessCnt);

    printf("\nDO Operation has been stopped...\n");

    if(!AutoResetBuf)
        DO_ContBufferReset(card);
    Release_Card(card);

    printf("\nPress any key to exit...\n");
    getch();
    return 0;
}
