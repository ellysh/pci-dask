/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2010/01/07                                                       */
/*                                                                            */
/* This sample performs continuous AI acquisition with Infinite Re-Trigger.   */
/* There are 32(Single Ended Mode)/16(Differetial Mode) AI channels in        */
/* PCI-9223. You can use polling mode or DMA to acquired data from specified  */
/* channels.                                                                  */
/* With infinite retrigger mode, there are two buffer should be setup for     */
/* infinite acquisition.                                                      */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <string.h>
#include "dask.h"

#define AI_ACQCNT 4000
#define AI_BUFCNT AI_ACQCNT
#define CHANNELCOUNT 4
#define P922X_TIMEBASE 80000000.0

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 ConfigCtrl = P922x_AI_SingEnded|P922x_AI_CONVSRC_INT;
    U16 TrigCtrl = P922x_AI_TRGMOD_POST|P922x_AI_TRGSRC_GPI0|P922x_AI_EnReTigger;
    U32 ReTriggerCount = 0; /*0: Infinite Trigger if retrigger is enabled*/
    BOOLEAN AutoResetBuf = 1;
    U32 ScanIntrv = 20000*CHANNELCOUNT; /*Scan Rate: P922X_TIMEBASE/20000/4=1K*/
    U32 SampIntrv = 20000; /*Sampling Rate: P922X_TIMEBASE/20000=4K*/
    U16 RDBuffer0[AI_BUFCNT]; /*data Buffer to be setup*/
    U16 RDBuffer1[AI_BUFCNT]; /*data Buffer to be setup*/
    U16 BufferId0;
    U16 BufferId1;
    U32 AI_ReadCount = AI_ACQCNT;
    U16 NumChans = CHANNELCOUNT; /*AI Channel Number*/
    U16 Chans[CHANNELCOUNT]; /*Array of AI Channels to be read*/
    U16 AdRanges[CHANNELCOUNT]; /*Array of AI Ranges*/
    BOOLEAN Ready;
    BOOLEAN StopFlag;
    U16 RdyTrigCnt;
    U32 AccessCnt;
    U8 FileName[] = "ai_data";
    U32 UserTrigCount = 0;
    U32 i, j;
    /*--------------------------------*/

    memset(RDBuffer0, '\0', AI_BUFCNT*sizeof(U16));
    memset(RDBuffer1, '\0', AI_BUFCNT*sizeof(U16));
    srand(time(NULL));
    i = 0;
    do{
        Chans[i] = rand()%32;
        for(j=0; j<i; j++){
            if(Chans[i]==Chans[j])
                break;
        }
        if(j!=i)
            continue;
        if(i%2==0)
            AdRanges[i] = AD_B_10_V;
        else
            AdRanges[i] = AD_B_5_V;
        i++;
    }while(i<NumChans);

    printf("This sample performs continuous AI acquisition from randon %d AI Channels\n", NumChans);
    printf("with infinite external digital trigger at %6.3lf Hz sampling rate.\n\n", P922X_TIMEBASE/ScanIntrv);

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

    /*Setup Buffer for AI DMA Transfer*/
    err = AI_ContBufferSetup(card, RDBuffer0, AI_BUFCNT, &BufferId0);
    if(err<0){
        printf("AI_ContBufferSetup0 Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    err = AI_ContBufferSetup(card, RDBuffer1, AI_BUFCNT, &BufferId1);
    if(err<0){
        printf("AI_ContBufferSetup1 Error: %d\n", err);
        AI_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    /*AI Acquisition Start*/
    printf("\nPress any key to start AI operation.\n");
    printf("You can press Enter to stop...\n");
    getch();
    err = AI_ContReadMultiChannelsToFile(card, NumChans, Chans, AdRanges, FileName, AI_ReadCount, 0/*Ignore*/, ASYNCH_OP);
    if(err<0){
        printf("AI_ContReadChannel Error: %d\n", err);
        AI_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }
    do{
        /*Check ReTrigger Ready and AI Acquisition End*/
        err = AI_AsyncReTrigNextReady(card, &Ready, &StopFlag, &RdyTrigCnt);
        if(err<0){
            printf("AI_AsyncReTrigNextReady Error: %d\n", err);
            AI_AsyncClear(card, &AccessCnt);
            AI_ContBufferReset(card);
            Release_Card(card);
            exit(1);
        }
        if(Ready){
            UserTrigCount++;
            /*Trigger Ready with Next Trigger*/
            printf("\nReady: %d, StopFlag: %d, TrigCnt: %d\n", Ready, StopFlag, RdyTrigCnt);
            if(RdyTrigCnt==0){
                printf("  Buffer 0 ready...\n");
                printf("  Write %d samples of Buffer0 to %s.dat file...\n", AI_ReadCount, FileName);
                AI_AsyncDblBufferToFile(card);
            }
            else{
                printf("  Buffer 1 ready...\n");
                printf("  Write %d samples of Buffer1 to %s.dat file...\n", AI_ReadCount, FileName);
                AI_AsyncDblBufferToFile(card);
            }
        }
    }while((!kbhit())&&(!StopFlag));

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
