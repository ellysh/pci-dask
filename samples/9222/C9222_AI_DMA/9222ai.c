/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2010/01/07                                                       */
/*                                                                            */
/* This sample performs continuous AI acquisition.                            */
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

#define AI_COUNT 4000
#define P922X_TIMEBASE 80000000.0
#define AI_SYNCTIMEOUT 1000

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 ConfigCtrl = P922x_AI_SingEnded|P922x_AI_CONVSRC_INT;
    U16 TrigCtrl = P922x_AI_TRGMOD_POST|P922x_AI_TRGSRC_SOFT;
    U32 ReTriggerCount = 0; /*Ignore in Non-Retrigger*/
    BOOLEAN AutoResetBuf = 1; /*Auto Reset Buffer Descriptor in Driver*/
    U32 ScanIntrv = 320; /*Scan Rate: P922X_TIMEBASE/320 = 250K Hz*/
    U32 SampIntrv = 320; /*Sampling Rate: P922X_TIMEBASE/320 = 250K Hz*/
    U16 RDBuffer[AI_COUNT];
    U16 BufferId;
    F64 VBuffer[AI_COUNT];
    U32 AI_ReadCount = AI_COUNT; /*AI Read Count*/
    U16 Channel = 0; /*AI Channel Number to be read*/
    U16 AdRange = AD_B_10_V; /*AI range*/
    FILE *w_file;
    U8 FileName[] = "ai_data.dat";
    U32 i;
    U32 TimeOut_ms = AI_SYNCTIMEOUT;
    /*--------------------------------*/

    memset(RDBuffer, '\0', AI_COUNT*sizeof(U16));
    memset(VBuffer, '\0', AI_COUNT*sizeof(F64));
    if(!(w_file=fopen(FileName, "w"))){
        printf("file open error...\n");
        exit(1);
    }
    fprintf(w_file, "Channel %d,\n", Channel);

    printf("This sample performs continuous AI acquisition from AI Channel %d\n", Channel);
    printf("at %6.3lf Hz sampling rate.\n\n", P922X_TIMEBASE/SampIntrv);

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

    /*Set Timeout Period for AI Sync Mode*/
    err = AI_SetTimeOut(card, TimeOut_ms);
    if(err<0){
        printf("AI_SetTimeOut Error: %d\n", err);
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
    err = AI_ContBufferSetup(card, RDBuffer, AI_ReadCount, &BufferId);
    if(err<0){
        printf("AI_ContBufferSetup Error: %d\n", err);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }

    /*AI Acquisition Start*/
    err = AI_ContReadChannel(card, Channel, AdRange, &BufferId, AI_ReadCount, 0/*Ignore*/, SYNCH_OP);
    if(err<0){
        printf("AI_ContReadChannel Error: %d\n", err);
        AI_ContBufferReset(card);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }

    printf("\n\nAI Acquisition Done... Acquired %d samples...\n", AI_ReadCount);
    printf("Write %d samples of Buffer to %s file...\n", AI_ReadCount, FileName);
    AI_ContVScale(card, AdRange, RDBuffer, VBuffer, AI_ReadCount);
    for(i=0; i<AI_ReadCount; i++){
        fprintf(w_file, "%f,\n", VBuffer[i]);
    }

    if(!AutoResetBuf)
        AI_ContBufferReset(card);
    Release_Card(card);
    fclose(w_file);

    printf("\nPress any key to exit...\n");
    getch();
    return 0;
}
