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
    U32 Buffer[WriteCount];
    U16 BufferId;
    int i, j;
    BOOLEAN Stopped = FALSE;
    U32 AccessCnt = 0;

    printf("\nThis sample performs %d AO in channel 0\n", WriteCount);
    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    for(i=0; i<WriteCount; i++){
        Buffer[i] = i*65535/WriteCount;
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

    err = AO_ContBufferSetup(card, Buffer, WriteCount, &BufferId);
    if(err<0){
        printf("AO_ContBufferSetup error: %d\n", err);
        Release_Card(card);
        exit(-1);
    }

    printf("\nPress any key to start.\n");
    getch();
    err = AO_ContWriteChannel(card, 0, BufferId, WriteCount, 1, SampIntrv, 1, ASYNCH_OP);
    if(err<0){
        printf("AO_ContWriteChannel error! err _code = %d\n", err);
        if(!AutoResetBuf)
            AO_ContBufferReset(card);
        Release_Card(card);
        exit(-1);
    }

    do{
        err = AO_AsyncCheck(card, &Stopped, &AccessCnt);
        if(err<0){
            printf("AO_AsyncCheck Error: %d\n", err);
            break;
        }
    }while((!Stopped)&&(!kbhit()));

    if(Stopped)
        printf("\nAO Done...\n");
    else
        printf("\nAO is stopped manually...\n");

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
