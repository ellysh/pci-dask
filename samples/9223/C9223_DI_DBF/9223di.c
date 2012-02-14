/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2010/01/07                                                       */
/*                                                                            */
/* This sample performs infinite DI acquisition with double buffer mode.      */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <string.h>
#include "dask.h"

#define DI_COUNT 4000

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 ConfigCtrl = P922x_DI_CONVSRC_INT;
    U16 TrigCtrl = P922x_DI_TRGMOD_POST|P922x_DI_TRGSRC_SOFT;
    U32 ReTriggerCount = 0; //Ignore in non-retrigger mode
    BOOLEAN AutoResetBuf = 1; //Auto reset buffer descriptor in driver
    U16 RDBuffer0[DI_COUNT]; //Data buffer
    U16 RDBuffer1[DI_COUNT]; //Data buffer
    U16 BufferId0;
    U16 BufferId1;
    U32 DI_ReadCount = DI_COUNT; //DI Read Count
    U16 Port = 0; //9223 supports only one DI port, port 0
    F64 SampleRate = 4000;
    BOOLEAN HalfReady;
    U32 AccessCnt;
    U16 BufRdyIdx = 0;
    U16 overrunFlag;
    FILE *w_file;
    U8 FileName[] = "di_data.dat";
    U32 vi;

    memset(RDBuffer0, '\0', DI_COUNT*sizeof(U16));
    memset(RDBuffer1, '\0', DI_COUNT*sizeof(U16));
    if(!(w_file=fopen(FileName, "w"))){
        printf("file open error...\n");
        exit(1);
    }

    printf("This sample performs infinite DI acquisition from DI Port with \n");
    printf("Double Buffer Mode at %6.3lf Hz sampling rate.\n\n", SampleRate);

    /*Open and Initialize Device*/
    printf("Card Number? ");
    scanf(" %hd", &card_num);
    card = Register_Card(PCI_9223, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        fclose(w_file);
        exit(1);
    }

    /*Configure DI*/
    err = DI_9223_Config(card, ConfigCtrl, TrigCtrl, ReTriggerCount, AutoResetBuf);
    if(err<0){
        printf("DI_9223_Config Error: %d\n", err);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }

    /*Enable Double Buffer Mode*/
    err = DI_AsyncDblBufferMode(card, 1);
    if(err<0){
        printf("DI_AsyncDblBufferMode Error: %d\n", err);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }

    /*Setup Buffer for DI DMA Transfer*/
    err = DI_ContBufferSetup(card, RDBuffer0, DI_ReadCount, &BufferId0);
    if(err<0){
        printf("DI_ContBufferSetup0 Error: %d\n", err);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }
    err = DI_ContBufferSetup(card, RDBuffer1, DI_ReadCount, &BufferId1);
    if(err<0){
        printf("DI_ContBufferSetup1 Error: %d\n", err);
        DI_ContBufferReset(card);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }

    /*DI Acquisition Start*/
    printf("\nPress any key to start DI operation.\n");
    getch();
    err = DI_ContReadPort(card, Port, &BufferId0, DI_ReadCount, SampleRate, ASYNCH_OP);
    if(err<0){
        printf("DI_ContReadPort Error: %d\n", err);
        DI_ContBufferReset(card);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }
    do{
        /*Check whether DI Acquisition is done*/
        err = DI_AsyncDblBufferHalfReady(card, &HalfReady);
        if(err<0){
            printf("DI_AsyncDblBufferHalfReady Error: %d\n", err);
            DI_AsyncClear(card, &AccessCnt);
            DI_ContBufferReset(card);
            Release_Card(card);
            fclose(w_file);
            exit(1);
        }
        if(HalfReady){
            if(BufRdyIdx==0){
                printf("\nBuffer 0 Half Ready...\n");
                printf("Write %d samples of Buffer0 to %s file...\n", DI_ReadCount, FileName);
                printf("                  Press Enter to stop...\n");
                for(vi=0; vi<DI_ReadCount; vi++)
                    fprintf(w_file, "0x%x\n", RDBuffer0[vi]);
                DI_AsyncDblBufferHandled(card); //Let driver to know buffer 0 is processed
                DI_AsyncDblBufferOverrun(card, 0, &overrunFlag);
                if(overrunFlag){
                    printf("Overrun...\n");
                    DI_AsyncDblBufferOverrun(card, 1, NULL);
                }
                BufRdyIdx = 1;
            }
            else{
                printf("\nBuffer 1 Half Ready...\n");
                printf("Write %d samples of Buffer1 to %s file...\n", DI_ReadCount, FileName);
                printf("                  Press Enter to stop...\n");
                for(vi=0; vi<DI_ReadCount; vi++)
                    fprintf(w_file, "0x%x\n", RDBuffer1[vi]);
                DI_AsyncDblBufferHandled(card); //Let driver to know buffer 1 is processed
                DI_AsyncDblBufferOverrun(card, 0, &overrunFlag);
                if(overrunFlag){
                    printf("Overrun...\n");
                    DI_AsyncDblBufferOverrun(card, 1, NULL);
                }
                BufRdyIdx = 0;
            }
        }
    }while(!kbhit());

    /*Clear DI Setting and Get Remaining data*/
    err = DI_AsyncClear(card, &AccessCnt);
    if(err<0){
        printf("DI_AsyncClear Error: %d\n", err);
        DI_AsyncClear(card, &AccessCnt);
        DI_ContBufferReset(card);
        Release_Card(card);
        fclose(w_file);
        exit(1);
    }

    if(BufRdyIdx==0){
        printf("\nDI Acquisition is stopped...\n");
        printf("Write last %d samples of Buffer0 to %s file...\n", AccessCnt, FileName);
        for(vi=0; vi<AccessCnt; vi++)
            fprintf(w_file, "0x%x\n", RDBuffer0[vi]);
    }
    else{
        printf("\nDI Acquisition is stopped...\n");
        printf("Write last %d samples of Buffer1 to %s file...\n", AccessCnt, FileName);
        for(vi=0; vi<AccessCnt; vi++)
            fprintf(w_file, "0x%x\n", RDBuffer1[vi]);
    }

    if(!AutoResetBuf)
        DI_ContBufferReset(card);
    Release_Card(card);
    fclose(w_file);

    printf("\nPress any key to exit...\n");
    getch();
    return 0;
}
