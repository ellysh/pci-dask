#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include "dask.h"

#define BufSize 32000
#define NumChans 16
#define SampIntrv 1000
#define ScanIntrv SampIntrv*NumChans

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 ConfigCtrl = P9221_AI_SingEnded;
    U16 TrigCtrl = P9221_TRGMOD_ExtD|P9221_TRGSRC_GPI0;
    BOOLEAN AutoResetBuf = TRUE;
    U16 Buffer1[BufSize];
    U16 Buffer2[BufSize];
    U16 BufferId1, BufferId2;
    U16 Chans[NumChans];
    U16 AdRanges[NumChans];
    BOOLEAN HalfReady = FALSE;
    BOOLEAN StopFlag = FALSE;
    BOOLEAN overrunFlag = FALSE;
    U32 AccessCnt = 0;
    int Loop;
    int viewIndex;
    int i;

    U8 *FileName = "acq";

    for(i=0; i<NumChans; i++){
        Chans[i] = 15-i;
        AdRanges[i] = AD_B_5_V;
    }

    memset(Buffer1, '\0', BufSize*2);
    memset(Buffer2, '\0', BufSize*2);

    printf("\nThis sample continue acquires analog data from ch15 to ch0\n");
    printf("in %6.2fHz scan rate while the external trigger(GPI0) is trigged.\n\n", 40000000.0/((float)ScanIntrv));

    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if((card=Register_Card(PCI_9221, card_num)) < 0){
        printf("Register_Card error=%d", card);
        exit(1);
    }

    err = AI_9221_Config(card, ConfigCtrl, TrigCtrl, AutoResetBuf);
    if(err < 0){
        printf("AI_9221_Config error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    err = AI_9221_CounterInterval(card, ScanIntrv, SampIntrv);
    if(err < 0){
        printf("AI_9221_CounterInterval error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    AI_AsyncDblBufferMode(card, 1);
    if(err < 0){
        print("AI_AsyncDblBufferMode error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    err = AI_ContBufferSetup(card, Buffer1, BufSize, &BufferId1);
    if(err < 0){
        printf("AI_ContBufferSetup1 error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    err = AI_ContBufferSetup(card, Buffer2, BufSize, &BufferId2);
    if(err < 0){
        printf("AI_ContBufferSetup2 error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    printf("\nPress any key to start.\n");
    printf("And the Acquired data will be stored in %s.dat\n", FileName);
    getch();
    err = AI_ContReadMultiChannelsToFile(card, NumChans, Chans, AdRanges, \
    FileName, BufSize, 0.0, ASYNCH_OP);
    if(err < 0){
        printf("AI cont. Read Multi Channels error\n");
        Release_Card(card);
        exit(1);
    }
    printf("\nAcquisition....  Press Enter to stop.\n");

    Loop = 0;
    viewIndex = 0;
    do{
        err = AI_AsyncDblBufferHalfReady(card, &HalfReady, &StopFlag);
        if(err<0){
            printf("AI_AsyncDblBufferHalfReady error: %d\n", err);
            break;
        }
        err = AI_AsyncDblBufferOverrun(card, 0, &overrunFlag);
        if(err<0)
            printf("AI_AsyncDblBufferOverrun error: %d\n", err);
        if(overrunFlag){
            printf("OVERRUN.. Please sets larger buffers\n");
            AI_AsyncDblBufferOverrun(card, 1, &overrunFlag);
        }
        if(HalfReady){
            printf("Buffer %d Half Ready, Write Data in Buffer %d to %s.dat file\n", viewIndex, viewIndex, FileName);
            err = AI_AsyncDblBufferToFile(card);
            if(err<0){
                printf("AI_AsyncDblBufferToFile error: %d\n", err);
            }
            viewIndex = (++viewIndex)%2;
            Loop++;
        }
    }while((!StopFlag)&&(!kbhit()));

    err = AI_AsyncClear(card, &AccessCnt);
    if(err<0){
        printf("AI_AsyncClear Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    printf("\nAcquisition stopped.\n");
    printf("Total Acuqired data: %d\n", BufSize*Loop+AccessCnt);

    if(!AutoResetBuf){
        err = AI_ContBufferReset(card);
        if(err < 0){
            printf("AI_ContBufferReset error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
    }

    Release_Card(card);
    printf("Press any key to exit...\n");
    getch();

    return 0;
}
