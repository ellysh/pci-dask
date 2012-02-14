/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2010/01/07                                                       */
/*                                                                            */
/* This sample performs infinite AI acquisition by double buffer mode.        */
/* There are 32(Single Ended Mode)/16(Differetial Mode) AI channels in        */
/* PCI-9223. You can use polling mode or DMA to acquired data from specified  */
/* channels.                                                                  */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <string.h>
#include "dask.h"

#define AI_COUNT 4000
#define P922X_TIMEBASE 80000000.0

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 ConfigCtrl = P922x_AI_SingEnded|P922x_AI_CONVSRC_INT;
    U16 TrigCtrl = P922x_AI_TRGMOD_POST|P922x_AI_TRGSRC_SOFT;
    U32 ReTriggerCount = 0; /*Ignore in Double Buffer Mode*/
    BOOLEAN AutoResetBuf = 1; /*Auto Reset Buffer Descriptor in Driver*/
    U32 ScanIntrv = 20000; /*Scan Rate: P922X_TIMEBASE/20000 = 4K Hz*/
    U32 SampIntrv = 20000; /*Sampling Rate: P922X_TIMEBASE/20000 = 4K Hz*/
    U16 RDBuffer0[AI_COUNT];
    U16 RDBuffer1[AI_COUNT];
    U32 AI_ReadCount = AI_COUNT;
    U16 BufferId0;
    U16 BufferId1;
    U16 Channel = 0;
    U16 AdRange = AD_B_10_V;
    BOOLEAN Stopped = FALSE;
    BOOLEAN HalfReady = FALSE;
    U32 AccessCnt = 0;
    U16 BufIdx = 0;
    U16 overrunFlag = 0;
    U8 FileName[] = "ai_data";
    /*--------------------------------*/

    memset(RDBuffer0, '\0', AI_COUNT*sizeof(U16));
    memset(RDBuffer1, '\0', AI_COUNT*sizeof(U16));

    printf("This sample performs infinite AI acquisition from AI Channel %d\n", Channel);
    printf("at %6.3lf Hz sampling rate by Double buffer mode.\n\n", P922X_TIMEBASE/SampIntrv);

    /*Open and Initialize Device*/
    printf("Card Number? ");
    scanf(" %hd", &card_num);
    card = Register_Card(PCI_9223, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    /*Configure AI*/
    err = AI_9223_Config(card, ConfigCtrl, TrigCtrl, ReTriggerCount, AutoResetBuf);
    if(err<0){
        printf("AI_9223_Config Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Set Scan and Sampling Rate*/
    err = AI_9223_CounterInterval(card, ScanIntrv, SampIntrv);
    if(err<0){
        printf("AI_9223_CounterInterval Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Enable Double Buffer Mode*/
    err = AI_AsyncDblBufferMode(card, 1);
    if(err<0){
        printf("AI_AsyncDblBufferMode Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Setup Buffer for AI DMA Transfer*/
    err = AI_ContBufferSetup(card, RDBuffer0, AI_ReadCount, &BufferId0);
    if(err<0){
        printf("AI_ContBufferSetup0 Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    err = AI_ContBufferSetup(card, RDBuffer1, AI_ReadCount, &BufferId1);
    if(err<0){
        printf("AI_ContBufferSetup1 Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*AI Acquisition Start*/
    err = AI_ContReadChannelToFile(card, Channel, AdRange, FileName, AI_ReadCount, 0/*Ignore*/, ASYNCH_OP);
    if(err<0){
        printf("AI_ContReadChannel Error: %d\n", err);
        AI_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }
    printf("\nAI Infinite Acquisition is started...\n");
    do{
        /*Check Buffer Ready*/
        err = AI_AsyncDblBufferHalfReady(card, &HalfReady, &Stopped);
        if(err<0){
            printf("AI_AsyncDblBufferHalfReady Error: %d\n", err);
            AI_AsyncClear(card, &AccessCnt);
            AI_ContBufferReset(card);
            Release_Card(card);
            exit(1);
        }
        if(HalfReady){
            if(BufIdx==0){
                /*
                 * The acquired AI data are stored in buffer 0,
                 * You can process the data of buffer 0 in HERE if you need.
                 */
                printf("\nBuffer0 Half Ready...\n");
                printf("Write %d samples of Buffer0 to %s.dat file...\n", AI_ReadCount, FileName);
                AI_AsyncDblBufferToFile(card);
                memset(RDBuffer0, '\0', AI_COUNT*sizeof(U16));
                AI_AsyncDblBufferOverrun(card, 0, &overrunFlag);
                if(overrunFlag){
                    printf("OVERRUN...\n");
                    AI_AsyncDblBufferOverrun(card, 1, &overrunFlag);
                }
                BufIdx = 1;
                printf("                              Press Enter to stop...\n");
            }
            else{
                /*
                 * The acquired AI data are stored in buffer 1,
                 * You can process the data of buffer 1 in HERE if you need.
                 */
                printf("\nBuffer1 Half Ready...\n");
                printf("Write %d samples of Buffer1 to %s.dat file...\n", AI_ReadCount, FileName);
                AI_AsyncDblBufferToFile(card);
                memset(RDBuffer1, '\0', AI_COUNT*sizeof(U16));
                AI_AsyncDblBufferOverrun(card, 0, &overrunFlag);
                if(overrunFlag){
                    printf("OVERRUN...\n");
                    AI_AsyncDblBufferOverrun(card, 1, &overrunFlag);
                }
                BufIdx = 0;
                printf("                              Press Enter to stop...\n");
            }
        }
    }while(!kbhit());

    /*Clear AI Setting and Get Remaining data*/
    err = AI_AsyncClear(card, &AccessCnt);
    if(err<0){
        printf("AI_AsyncClear Error: %d\n", err);
        AI_AsyncClear(card, &AccessCnt);
        AI_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    if(!AutoResetBuf)
        AI_ContBufferReset(card);
    Release_Card(card);

    printf("\nPress any key to exit...\n");
    getch();
    return 0;
}
