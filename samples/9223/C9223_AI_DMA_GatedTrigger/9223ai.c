/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2010/01/07                                                       */
/*                                                                            */
/* This sample performs AI acquisition with External Gated Trigger.           */
/* There are 32(Single Ended Mode)/16(Differetial Mode) AI channels in        */
/* PCI-9223. You can use polling mode or DMA to acquired data from specified  */
/* channels.                                                                  */
/* Gated Trigger - AI ADC will pause while the set external trigger is active */
/* For example, if trigger source is set to GPI0 and trigger polarity is set  */
/* to Positive, then PCI-9223 pauses acquisition while GPI0 is 1.             */
/* Note that the data bytes is 4 if gated trigger is set.                     */
/* Bit 0~15: Data, Bit 17: Separated bit.                                     */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <string.h>
#include "dask.h"

#define READCOUNT 25000
#define P922X_TIMEBASE 80000000.0

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 ConfigCtrl = P922x_AI_SingEnded|P922x_AI_CONVSRC_INT;
    U16 TrigCtrl = P922x_AI_TRGMOD_GATED|P922x_AI_TRGSRC_GPI0|P922x_AI_TrgPositive;
    U32 ReTriggerCount = 0; /*Ignore if trigger mode is gated trigger*/
    BOOLEAN AutoResetBuf = 1; /*Autoreset buffer descriptor build in driver*/
    U32 ScanIntrv = 320; /*Scan rate: P922X_TIMEBASE/320=250K*/
    U32 SampIntrv = 320; /*Sampling rate: P922X_TIMEBASE/320=250K*/
    U32 RDBuffer[READCOUNT]; /*4 Bytes per a sample if trigger mode is Gated Trigger*/
    U16 BufferId;
    F64 VBuffer[READCOUNT];
    U32 AI_ReadCount = READCOUNT; /*AI read count*/
    U16 Channel = 0; /*AI channel to be read*/
    U16 AdRange = AD_B_10_V; /*AI range*/
    BOOLEAN Stopped = FALSE;
    U32 AccessCnt = 0;
    FILE *w_file;
    U8 FileName[] = "ai_data.dat";
    U32 i;

    memset(RDBuffer, '\0', READCOUNT*sizeof(U32));
    memset(VBuffer, '\0', READCOUNT*sizeof(F64));
    if(!(w_file=fopen(FileName, "w"))){
        printf("file open error...\n");
        exit(1);
    }
    fprintf(w_file, "Channel %d,\n", Channel);
    printf("This sample performs continuous AI acquisition with Gated trigger\n");
    printf("from AI Channel %d at %6.3lf Hz sampling rate.\n", Channel, P922X_TIMEBASE/SampIntrv);

    /*Open and Initialize Devie*/
    printf("Card Number? ");
    scanf(" %hd", &card_num);
    card = Register_Card(PCI_9223, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        fclose(w_file);
        exit(1);
    }

    /*Configure AI configurations*/
    err = AI_9223_Config(card, ConfigCtrl, TrigCtrl, ReTriggerCount, AutoResetBuf);
    if(err<0){
        printf("AI_9223_Config Error: %d\n", err);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }

    /*Set Counter Scan and Sample Interval*/
    err = AI_9223_CounterInterval(card, ScanIntrv, SampIntrv);
    if(err<0){
        printf("AI_9223_CounterInterval Error: %d\n", err);
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

    /*Buffer Setup - Build Buffer Descriptor in Driver*/
    err = AI_ContBufferSetup(card, RDBuffer, AI_ReadCount, &BufferId);
    if(err<0){
        printf("AI_ContBufferSetup Error: %d\n", err);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }

    /*Start AI Acquisition*/
    err = AI_ContReadChannel(card, Channel, AdRange, &BufferId, AI_ReadCount, 0/*Ignore*/, ASYNCH_OP);
    if(err<0){
        printf("AI_ContReadChannel Error: %d\n", err);
        AI_ContBufferReset(card);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }
    printf("\nWait Gated Positive Trigger from GPI0...\n");
    do{
        err = AI_AsyncCheck(card, &Stopped, &AccessCnt);
        if(err<0){
            printf("AI_AsyncCheck Error: %d\n", err);
            AI_AsyncClear(card, &AccessCnt);
            AI_ContBufferReset(card);
            Release_Card(card);
            fclose(w_file);
            exit(1);
        }
    }while((!kbhit())&&(!Stopped));

    err = AI_AsyncClear(card, &AccessCnt);
    if(err<0){
        printf("AI_AsyncClear Error: %d\n", err);
        AI_AsyncClear(card, &AccessCnt);
        AI_ContBufferReset(card);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }

    if(Stopped)
        printf("\n\nAI Acquisition done...\n");
    else
        printf("\n\nAI Acquisition has been stopped manually...\n");

    printf("Write %d samples of Buffer to %s file...\n\n", AccessCnt, FileName);
    fprintf(w_file, "Raw Data, Scaled Voltage,\n");
    AI_ContVScale(card, AdRange, RDBuffer, VBuffer, AccessCnt);
    for(i=0; i<AccessCnt; i++){
        fprintf(w_file, "0x%x, %6.4f,\n", RDBuffer[i], VBuffer[i]);
    }

    if(!AutoResetBuf)
        AI_ContBufferReset(card);
    Release_Card(card);
    fclose(w_file);

    printf("\nPress any key to exit...\n");
    getch();
    return 0;
}
