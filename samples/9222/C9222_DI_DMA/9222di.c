/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2010/01/07                                                       */
/*                                                                            */
/* This sample performs continuous DI acquisition.                            */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <string.h>
#include "dask.h"

#define DI_COUNT 4000
#define DI_SYNCTIMEOUT 1000

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 ConfigCtrl = P922x_DI_CONVSRC_INT;
    U16 TrigCtrl = P922x_DI_TRGMOD_POST|P922x_DI_TRGSRC_SOFT;
    U32 ReTriggerCount = 0; //Ignore in non-retrigger mode
    BOOLEAN AutoResetBuf = 1; //Auto reset buffer descriptor built in driver
    U16 RDBuffer[DI_COUNT];
    U16 BufferId;
    U32 DI_ReadCount = DI_COUNT; //DI Read Count
    U16 Port = 0; //9222 supports only one DI port, port 0
    F64 SampleRate = 2000000;
    FILE *w_file;
    U8 FileName[] = "di_data.dat";
    U32 i;
    U32 TimeOut_ms = DI_SYNCTIMEOUT;

    memset(RDBuffer, '\0', DI_COUNT*sizeof(U16));
    if(!(w_file=fopen(FileName, "w"))){
        printf("file open error...\n");
        exit(1);
    }

    printf("This sample performs continuous DI acquisition from DI Port\n");
    printf("at %6.3lf Hz sampling rate.\n\n", SampleRate);

    /*Open and Initialize Device*/
    printf("Card Number? ");
    scanf(" %hd", &card_num);
    card = Register_Card(PCI_9222, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        fclose(w_file);
        exit(1);
    }

    /*Configure DI*/
    err = DI_9222_Config(card, ConfigCtrl, TrigCtrl, ReTriggerCount, AutoResetBuf);
    if(err<0){
        printf("DI_9222_Config Error: %d\n", err);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }

    /*Set Timeout Period for AI Sync Mode*/
    err = DI_SetTimeOut(card, TimeOut_ms);
    if(err<0){
        printf("DI_SetTimeOut Error: %d\n", err);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }

    /*Disable Double Buffer Mode*/
    err = DI_AsyncDblBufferMode(card, 0);
    if(err<0){
        printf("DI_AsyncDblBufferMode Error: %d\n", err);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }

    /*Setup Buffer for DI DMA Transfer*/
    err = DI_ContBufferSetup(card, RDBuffer, DI_ReadCount, &BufferId);
    if(err<0){
        printf("DI_ContBufferSetup Error: %d\n", err);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }

    /*DI Acquisition Start*/
    err = DI_ContReadPort(card, Port, &BufferId, DI_ReadCount, SampleRate, SYNCH_OP);
    if(err<0){
        printf("DI_ContReadPort Error: %d\n", err);
        DI_ContBufferReset(card);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }

    printf("\n\nDI Acquisition Done... Acquired %d samples...\n", DI_ReadCount);
    printf("Write %d samples of Buffer to %s file...\n", DI_ReadCount, FileName);
    for(i=0; i<DI_ReadCount; i++){
        fprintf(w_file, "%#x,\n", RDBuffer[i]);
    }

    if(!AutoResetBuf)
        DI_ContBufferReset(card);
    Release_Card(card);
    fclose(w_file);

    printf("\nPress any key to exit...\n");
    getch();
    return 0;
}
