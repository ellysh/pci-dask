#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include "dask.h"

#define Channel 15
#define ScanCount 500
#define ReadCount (Channel+1)*ScanCount
#define SampIntrv 160
#define ScanIntrv SampIntrv*(Channel+1)

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 ConfigCtrl = P9221_AI_SingEnded;
    U16 TrigCtrl = P9221_TRGMOD_SOFT;
    BOOLEAN AutoResetBuf = TRUE;
    U16 Buffer[ReadCount];
    F64 ScaleBuffer[ReadCount];
    U16 BufferId;
    FILE* fout;
    int i, j;
    BOOLEAN Stopped = FALSE;
    U32 AccessCnt = 0;
    U16 AdRange = AD_B_5_V;

    if(!(fout=fopen("acq_data","w"))){
        printf("fopen Failed\n");
    }
    for(i=0; i<=Channel; i++)
        fprintf(fout, "Ch%d,   ", i);
    fprintf(fout, "\n");

    memset(Buffer, '\0', ReadCount*2);
    memset(ScaleBuffer, '\0', ReadCount*8);

    printf("\nThis sample performs %d scans from channel 0 to 15\n", ScanCount);
    printf("in %6.2f scan rate while software trigger is started\n\n", 40000000.0/((float)ScanIntrv));
    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if((card=Register_Card(PCI_9221, card_num))<0){
        printf("Register_Card error=%d", card);
        exit(1);
    }

    err = AI_9221_Config(card, ConfigCtrl, TrigCtrl, AutoResetBuf);
    if(err<0){
        printf("AI_9221_Config error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    err = AI_9221_CounterInterval(card, ScanIntrv, SampIntrv);
    if(err<0){
        printf("AI_9221_CounterInterval error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    err = AI_ContBufferSetup(card, Buffer, ReadCount, &BufferId);
    if(err < 0){
        printf("AI_ContBufferSetup error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    printf("\nPress any key to start.\n");
    getch();
    err = AI_ContScanChannels(card, Channel, AdRange, &BufferId, ReadCount, 0.0, ASYNCH_OP);
    if(err < 0){
        printf("AI cont Scan Read Channels error\n");
        Release_Card(card);
        exit(1);
    }

    i=0;
    do{
        err = AI_AsyncCheck(card, &Stopped, &AccessCnt);
        if(err<0){
            printf("AI_AsyncCheck Error: %d\n", err);
            break;
        }
    }while((!Stopped)&&(!kbhit()));
    printf("\nAcquisition Done...\n");

    err = AI_AsyncClear(card, &AccessCnt);
    if(err<0){
        printf("AI_AsyncClear Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    err = AI_ContVScale(card, AdRange, Buffer, ScaleBuffer, AccessCnt);
    if(err<0){
        printf("AI_ContVScale error: %d\n", err);
    }

    printf("\nTotal %d acquired data is stored in acq_data\n", AccessCnt);
    for(i=0; i<ScanCount; i++){
        for(j=0; j<(Channel+1); j++)
            fprintf(fout,"%4.2f, ", (float)(ScaleBuffer[i*(Channel+1)+j]));
        fprintf(fout, "\n");
    }

    if(!AutoResetBuf){
        err = AI_ContBufferReset(card);
        if(err < 0){
            printf("AI_ContBufferReset error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
    }

    fclose(fout);
    Release_Card(card);
    printf("\nPress any key to exit...\n");
    getch();

    return 0;
}
