/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2010/01/07                                                       */
/*                                                                            */
/* This sample performs continuous AI acquisition with Re-Trigger mode.       */
/* There are 16(Single Ended Mode)/8(Differetial Mode) AI channels in         */
/* PCI-9222. You can use polling mode or DMA to acquired data from specified  */
/* channels.                                                                  */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <string.h>
#include "dask.h"

#define AI_ACQCNT 4000                   /*Acquisition count per one trigger*/
#define AI_TRIGCNT 4                     /*Total Trigger count*/
#define AI_BUFCNT AI_ACQCNT*AI_TRIGCNT   /*buffer size for all acquired data*/
#define CHANNELCOUNT 4                   /*AI Channel Count*/
#define P922X_TIMEBASE 80000000.0        /*Internal Timebase of PCI-922x*/

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 ConfigCtrl = P922x_AI_SingEnded|P922x_AI_CONVSRC_INT;
    U16 TrigCtrl = P922x_AI_TRGMOD_POST|P922x_AI_TRGSRC_GPI0|P922x_AI_EnReTigger;
    U32 ReTriggerCount = AI_TRIGCNT-1; /*ReTrigger Count = Total Trigger Count - 1*/
    BOOLEAN AutoResetBuf = 1;
    U32 ScanIntrv = 320*CHANNELCOUNT; /*Scan Rate: P922X_TIMEBASE/320/4 = 62.5K Hz*/
    U32 SampIntrv = 320; /*Scan Rate: P922X_TIMEBASE/320 = 250K Hz*/
    U16 RDBuffer[AI_BUFCNT]; /*Buffer to be setup*/
    U16 BufferId;
    F64 VBuffer[AI_BUFCNT];
    U32 AI_ReadCount = AI_ACQCNT; /*AI Read Count per one Trigger*/
    U16 NumChans = CHANNELCOUNT; /*AI Channel Count*/
    U16 Chans[CHANNELCOUNT]; /*AI Channels Array*/
    U16 AdRanges[CHANNELCOUNT]; /*AI Ranges Array*/
    BOOLEAN Ready;
    BOOLEAN StopFlag;
    U16 RdyTrigCnt;
    U32 AccessCnt;
    FILE *w_file;
    U8 FileName[] = "ai_data.dat";
    U32 i, j;
    /*--------------------------------*/

    memset(RDBuffer, '\0', AI_BUFCNT*sizeof(U16));
    memset(VBuffer, '\0', AI_BUFCNT*sizeof(F64));
    if(!(w_file=fopen(FileName, "w"))){
        printf("file open error...\n");
        exit(1);
    }
    srand(time(NULL));
    i = 0;
    do{
        Chans[i] = rand()%16;
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
        fprintf(w_file, "Channel %d,", Chans[i]);
        i++;
    }while(i<NumChans);
    fprintf(w_file, "\n");

    printf("This sample performs continuous AI acquisition from randon %d AI Channels\n", NumChans);
    printf("with external digital retrigger at %6.3lf Hz scan rate.\n\n", P922X_TIMEBASE/ScanIntrv);

    /*Open and Initialize Device*/
    printf("Card Number? ");
    scanf(" %hd", &card_num);
    card = Register_Card(PCI_9222, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        fclose(w_file);
        exit(1);
    }

    /*Configure AI*/
    err = AI_9222_Config(card, ConfigCtrl, TrigCtrl, ReTriggerCount, AutoResetBuf);
    if(err<0){
        printf("AI_9222_Config Error: %d\n", err);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }

    /*Set Scan and Sampling Rate*/
    err = AI_9222_CounterInterval(card, ScanIntrv, SampIntrv);
    if(err<0){
        printf("AI_9222_CounterInterval Error: %d\n", err);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }

    /*Disable Double Buffer Mode*/
    err = AI_AsyncDblBufferMode(card, 0);
    if(err<0){
        printf("AI_AsyncDblBufferMode Error: %d\n", err);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }

    /*Setup Buffer for AI DMA Transfer*/
    err = AI_ContBufferSetup(card, RDBuffer, AI_BUFCNT/*Set all need buffe size*/, &BufferId);
    if(err<0){
        printf("AI_ContBufferSetup Error: %d\n", err);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }

    /*AI Acquisition Start*/
    printf("\nPress any key to start AI.\n");
    printf("You can press Enter to stop...\n");
    getch();
    err = AI_ContReadMultiChannels(card, NumChans, Chans, AdRanges, &BufferId, AI_ReadCount, 0/*Ignore*/, ASYNCH_OP);
    if(err<0){
        printf("AI_ContReadMultiChannels Error: %d\n", err);
        AI_ContBufferReset(card);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }
    printf("\nAI Acquisition is started...\n");
    printf("Wait 4 External Digital Trigger from GPI0...\n\n");
    do{
        /*Check ReTrigger Ready and AI Acquisition End*/
        err = AI_AsyncReTrigNextReady(card, &Ready, &StopFlag, &RdyTrigCnt);
        if(err<0){
            printf("AI_AsyncReTrigNextReady Error: %d\n", err);
            AI_AsyncClear(card, &AccessCnt);
            AI_ContBufferReset(card);
            Release_Card(card);
            fclose(w_file);
            exit(1);
        }
        if(Ready){
            /*Trigger Ready with Next Trigger*/
            printf("Trigger Ready: %d, AIEnd: %d, Trigger Count: %d\n", Ready, StopFlag, RdyTrigCnt);
        }
        if(StopFlag){
            /*AI End*/
            printf("Trigger Ready: %d, AIEnd: %d, Trigger Count: %d\n", Ready, StopFlag, RdyTrigCnt);
            break;
        }
    }while(!kbhit());

    /*Clear AI Setting and Get Remaining data*/
    err = AI_AsyncClear(card, &AccessCnt);
    if(err<0){
        printf("AI_AsyncClear Error: %d\n", err);
        AI_AsyncClear(card, &AccessCnt);
        AI_ContBufferReset(card);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }

    if(StopFlag){
        printf("\nAI Acquisition Done... Acquired %d samples...\n", AccessCnt);
        printf("Write %d samples of Buffer to %s file...\n", AccessCnt, FileName);
        for(i=0; i<AccessCnt/NumChans; i++){
            for(j=0; j<NumChans; j++){
                AI_VoltScale(card, AdRanges[j], (U16)(RDBuffer[i*NumChans+j]), &VBuffer[i*NumChans+j]);
                fprintf(w_file, "%f,  ", VBuffer[i*NumChans+j]);
            }
            fprintf(w_file,"\n");
        }
    }
    else{
        printf("\nAI Acquisition has been stopped... Acquired %d samples...\n", AccessCnt);
        printf("Write %d samples of Buffer0 to %s file...\n", AccessCnt, FileName);
        for(i=0; i<AccessCnt/NumChans; i++){
            for(j=0; j<NumChans; j++){
                AI_VoltScale(card, AdRanges[j], (U16)(RDBuffer[i*NumChans+j]), &VBuffer[i*NumChans+j]);
                fprintf(w_file, "%f,  ", VBuffer[i*NumChans+j]);
            }
            fprintf(w_file,"\n");
        }
    }

    if(!AutoResetBuf)
        AI_ContBufferReset(card);
    Release_Card(card);
    fclose(w_file);

    printf("\nPress any key to exit...\n");
    getch();
    return 0;
}
