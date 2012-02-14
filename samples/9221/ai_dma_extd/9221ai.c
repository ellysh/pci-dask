#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include "dask.h"

#define ReadCount 4096
#define ScanIntrv 160
#define SampIntrv 160

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 ConfigCtrl = P9221_AI_Differential;
    U16 TrigCtrl = P9221_TRGMOD_ExtD|P9221_TRGSRC_GPI0;
    BOOLEAN AutoResetBuf = TRUE;
    U16 Buffer[ReadCount];
    U16 BufferId;
    U16 Channel = 0;
    U16 AdRange = AD_B_5_V;
    F64 voltage = 0.0;
    FILE* fout;
    int i;

    if(!(fout=fopen("acq_data","w"))){
        printf("fopen Failed\n");
    }
    memset(Buffer, '\0', ReadCount*2);

    printf("\nThis sample acquires %d data from AI Channel %d (Differential Mode)\n", ReadCount, Channel);
    printf("in %6.2fHz sampling rate while exiernal trigger(GPI0) is trigged.\n\n", 40000000.0/((float)SampIntrv));
    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if((card=Register_Card(PCI_9221, card_num)) < 0) {
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

    err = AI_SetTimeOut(card, 60000); //Set timeout 60sec
    if(err < 0){
        printf("AI_SetTimeOut error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    err = AI_ContBufferSetup(card, Buffer, ReadCount, &BufferId);
    if(err < 0){
        printf("AI_ContBufferSetup error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    printf("\nPress any key to Start.\n");
    getch();
    err = AI_ContReadChannel(card, Channel, AdRange, &BufferId, ReadCount, 0.0, SYNCH_OP);
    if(err<0){
        printf("AI count Read Channels error\n");
        Release_Card(card);
        exit(1);
    }
    printf("\nAcquisition Done...\n");

    err = AI_ContBufferReset(card);
    if(err < 0){
        printf("AI_ContBufferReset error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    printf("The acquired data will be stored in acq_data\n");
    for(i=0; i<ReadCount; i++){
        err = AI_VoltScale(card, AD_B_5_V, Buffer[i], &voltage);
        if(err < 0)
            printf("AI_VoltScale Failed\n");
        fprintf(fout,"%f\n", (float)voltage);
    }

    fclose(fout);
    Release_Card(card);
    printf("\nPress any key to exit...\n");
    getch();

    return 0;
}
