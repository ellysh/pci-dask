#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include "dask.h"

#define WriteCount 1024
#define SampIntrv 80000

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 ConfigCtrl = P6202_DA_WRSRC_Int;
    U16 TrigCtrl = P6202_DA_TRGSRC_SOFT;
    BOOLEAN AutoResetBuf = FALSE;
    U32 Buffer_0[WriteCount];
    U32 Buffer_1[WriteCount];
    U16 BufferId_0;
    U16 BufferId_1;
    int i, j;
    BOOLEAN Stopped = FALSE;
    U32 AccessCnt = 0;

    printf("\nThis sample performs AO in channel 0\n");
    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    for(i=0; i<WriteCount; i++){
        Buffer_0[i] = i*65535/WriteCount;
        Buffer_1[i] = 65535-Buffer_0[i];
    }

    if((card = Register_Card(PCI_6202, card_num))<0){
        printf("Register_Card %d error: %d", card_num, card);
        exit(-1);
    }

    err = AO_6202_Config(card, ConfigCtrl, TrigCtrl, 1, 0, 0, AutoResetBuf);
    if(err<0){
        printf("AO_6202_Config error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    err = AO_AsyncDblBufferMode(card, TRUE);
    if(err<0){
        printf("AO_AsyncDblBufferMode error: %d\n", err);
        Release_Card(card);
        exit(-1);
    }

    err = AO_ContBufferSetup(card, Buffer_0, WriteCount, &BufferId_0);
    if(err<0){
        printf("AO_ContBufferSetup1 error: %d\n", err);
        Release_Card(card);
        exit(-1);
    }
    err = AO_ContBufferSetup(card, Buffer_1, WriteCount, &BufferId_1);
    if(err<0){
        printf("AO_ContBufferSetup2 error: %d\n", err);
        if(!AutoResetBuf)
            AO_ContBufferReset(card);
        Release_Card(card);
        exit(-1);
    }

    printf("\nPress any key to start.\n");
    getch();
    err = AO_ContWriteChannel(card, 0, BufferId_0, WriteCount, 1, SampIntrv, 1, ASYNCH_OP);
    if(err<0){
        printf("AO_ContWriteChannel error! err _code = %d\n", err);
        if(!AutoResetBuf)
            AO_ContBufferReset(card);
        Release_Card(card);
        exit(-1);
    }

    printf("\nPress any key to stop.\n");
    getch();
    err = AO_AsyncClear(card, &AccessCnt, 0);
    if(err<0){
        printf("AO_AsyncClear Error: %d\n", err);
    }

    if(!AutoResetBuf){
        err = AO_ContBufferReset(card);
        if(err<0){
            printf("AO_ContBufferReset error: %d\n", err);
        }
    }

    Release_Card(card);
    printf("\nPress any key to exit...\n");
    getch();
    return 0;
}
