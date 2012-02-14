/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2010/01/07                                                       */
/*                                                                            */
/* This sample performs continuous AI acquisition.                            */
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

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 ConfigCtrl = P922x_AI_SingEnded|P922x_AI_CONVSRC_GPI0;
    U16 TrigCtrl = 0; /*Dose not support any trigger mode/source in external conversion source mode*/
    U32 ReTriggerCount = 0; /*Dose not support any trigger mode/source in external conversion source mode*/
    BOOLEAN AutoResetBuf = 1; /*Auto Reset Buffer Descriptor in Driver*/
    U16 RDBuffer[AI_COUNT];
    U16 BufferId;
    F64 VBuffer[AI_COUNT];
    U32 AI_ReadCount = AI_COUNT; /*AI Read Count*/
    U16 Channel = 0; /*AI Channel Number to be read*/
    U16 AdRange = AD_B_10_V; /*AI range*/
    BOOLEAN Stopped;
    U32 AccessCnt;
    FILE *w_file;
    U8 FileName[] = "ai_data.dat";
    U32 i;
    /*--------------------------------*/

    memset(RDBuffer, '\0', AI_COUNT*sizeof(U16));
    memset(VBuffer, '\0', AI_COUNT*sizeof(F64));
    if(!(w_file=fopen(FileName, "w"))){
        printf("file open error...\n");
        exit(1);
    }
    fprintf(w_file, "Channel %d,\n", Channel);

    printf("This sample performs continuous AI acquisition from AI Channel %d\n", Channel);
    printf("with external conversion source (GPI0).\n\n");

    /*Open and Initialize Device*/
    printf("Card Number? ");
    scanf(" %hd", &card_num);
    card = Register_Card(PCI_9223, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        fclose(w_file);
        exit(1);
    }

    /*Configure AI*/
    err = AI_9223_Config(card, ConfigCtrl, TrigCtrl, ReTriggerCount, AutoResetBuf);
    if(err<0){
        printf("AI_9223_Config Error: %d\n", err);
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
    printf("\nPress any key to start AI to wait conversion source from GPI0.\n");
    printf("                            And you can press Enter to stop...\n");
    getch();
    err = AI_ContReadChannel(card, Channel, AdRange, &BufferId, AI_ReadCount, 0/*Ignore*/, ASYNCH_OP);
    if(err<0){
        printf("AI_ContReadChannel Error: %d\n", err);
        AI_ContBufferReset(card);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }
    do{
        /*Check whether AI Acquisition is done*/
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

    if(Stopped){
        printf("\n\nAI Acquisition Done... Acquired %d samples...\n", AccessCnt);
        printf("Write %d samples of Buffer to %s file...\n", AccessCnt, FileName);
        AI_ContVScale(card, AdRange, RDBuffer, VBuffer, AccessCnt);
        for(i=0; i<AccessCnt; i++){
            fprintf(w_file, "%f,\n", VBuffer[i]);
        }
    }
    else{
        printf("\nAI Acquisition has been stopped manually... Acquired %d samples...\n", AccessCnt);
        printf("Write %d samples of Buffer to %s file...\n", AccessCnt, FileName);
        AI_ContVScale(card, AdRange, RDBuffer, VBuffer, AccessCnt);
        for(i=0; i<AccessCnt; i++){
            fprintf(w_file, "%f,\n", VBuffer[i]);
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
