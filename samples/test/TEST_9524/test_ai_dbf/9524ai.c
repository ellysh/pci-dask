/*----------------------------------------------------------------------------*/
/* Date: 2009/08/19                                                           */
/* Company: ADLINK Technology Inc.                                            */
/*                                                                            */
/* This sample setups two system buffer to perform infinitely continuous AI   */
/* read.                                                                      */
/* Step1. Setup two buffers and configure AI                                  */
/* Step2. Start AI                                                            */
/* Step3. Wait Buffer 0 Full                                                  */
/* Step4. You can process the data of buffer while buffer 0 is full, since    */
/*        the DMA chip accesses buffer 1 at that time. Note that you should   */
/*        complete your data process before the next buffer (Buffer 1) is     */
/*        full(Since Buffer 1 is full, the DMA chip accesses Buffer 0).       */
/* Step5. Wait Buffer 1 Full                                                  */
/* Step6. You can process the data of buffer while buffer 1 is full, since    */
/*        the DMA chip accesses buffer 0 at that time. Note that you should   */
/*        complete your data process before the next buffer (Buffer 0) is     */
/*        full(Since Buffer 0 is full, the DMA chip accesses Buffer 1).       */
/* Step7. repeat Step3 to Step6                                               */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include "dask.h"

#define READCOUNT 60

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    FILE *fin;
    int i;
    U32 ReadCount = READCOUNT;
    U32 Buffer1[READCOUNT];
    U16 BufID1;
    U32 Buffer2[READCOUNT];
    U16 BufID2;
    U16 Group;
    U16 XMode = P9524_AI_XFER_DMA;
    U16 ADC_Range;
    U16 ADC_SampRate = P9524_ADC_30K_SPS;
    U16 ConfigCtrl;
    U16 TrigCtrl = 0;
    U32 dwTrigValue = 0;
    U16 Channel;
    F64 voltageArray[READCOUNT];
    F64 ActualRate;
    BOOLEAN Stopped = 0;
    BOOLEAN HalfReady = 0;
    U32 AccessCnt;
    int viewidx = 0;
    U16 overrunFlag = 0;
    U32 BufRdyCnt = 0, OvrnCnt = 0;

    memset(Buffer1, '\0', ReadCount*4);
    memset(Buffer2, '\0', ReadCount*4);
    memset(voltageArray, '\0', ReadCount*8);

    printf("This sample performs Continuous AI operation by Double-Buffer Mode.\n");
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    /*Open and initialize device*/
    card = Register_Card(PCI_9524, card_num);
    if(card<0){
        printf("Register Card Failed: %d\n", card);
        exit(1);
    }

    printf("Load Cell Channel[0] or General Purpose Channel[1]? [0]or[1] ");
    scanf(" %hd", &Group);
    if(Group==0){
        /*Load Cell AI*/
        ADC_Range = 0;
        ConfigCtrl = P9524_VEX_Range_10V|P9524_VEX_Sence_Remote|P9524_AI_AZMode|P9524_AI_BufAutoReset;
        Channel = P9524_AI_LC_CH0;
    }
    else if(Group==1){
        /*General Purpose AI*/
        ADC_Range = AD_B_10_V;
        ConfigCtrl = P9524_AI_BufAutoReset;
        Channel = P9524_AI_GP_CH0;
    }
    else{
        printf("invalid group\n");
        Release_Card(card);
        exit(1);
    }

    /*Enable double buffer mode*/
    err = AI_AsyncDblBufferMode(card, 1);
    if(err<0){
        printf("AI_AsyncDblBufferMode Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Setup buffer for DAM transfer*/
    err = AI_ContBufferSetup(card, Buffer1, ReadCount, &BufID1);
    if(err<0){
        printf("AI_ContBufferSetup Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    err = AI_ContBufferSetup(card, Buffer2, ReadCount, &BufID2);
    if(err<0){
        printf("AI_ContBufferSetup2 Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    if(Group==0){
        /*Load Cell AI need extra DSP configuration*/
        U16 DFStage = 5;
        U32 SPKRejThreshold = 268;
        err = AI_9524_SetDSP(card, Channel, P9524_SPIKE_REJ_ENABLE, DFStage, SPKRejThreshold);
        if(err<0){
            printf("AI_9524_SetDSP Error: %d\n", err);
            AI_ContBufferReset(card);
            Release_Card(card);
            exit(1);
        }
    }

    /*Configure AI*/
    err = AI_9524_Config(card, Group, XMode, ConfigCtrl, TrigCtrl, dwTrigValue);
    if(err<0){
        printf("AI_9524_Config Error: %d\n", err);
        AI_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }

    if(!(fin=fopen("acq.dat", "w"))){
        printf("fopen error\n");
    }

    printf("\nPress any key to start AI Acquisition\n");
    getch();
    err = AI_ContReadChannel(card, Channel, ADC_Range, &BufID1, ReadCount, (F64)ADC_SampRate, ASYNCH_OP);
    if(err<0){
        printf("AI_ContReadChannel Error: %d\n", err);
        if(fin)
            fclose(fin);
        AI_ContBufferReset(card);
        Release_Card(card);
        exit(1);
    }
    clrscr();
    /*Process Double Buffer Mode*/
    do{
        err = AI_AsyncDblBufferHalfReady(card, &HalfReady, &Stopped);
        if(err<0){
            printf("AI_AsyncCheck Error: %d\n", err);
            if(fin)
                fclose(fin);
            AI_ContBufferReset(card);
            Release_Card(card);
            exit(1);
        }
        if(HalfReady){
            BufRdyCnt++;
            if(!viewidx){
                //printf("Buffer 1 HalfReady, Process the data of Buffer 1\n");
                /*Process buffer 1 data*/
                AI_ContVScale(card, ADC_Range, Buffer1, voltageArray, ReadCount);
                /*
                for(i=0; i<ReadCount; i++){
                    if(fin)
                        fprintf(fin, "%13.9f\n", voltageArray[i]);
                }
                */
                viewidx = 1;
                /*Tell the driver you complete the buffer 1 process*/
                err = AI_AsyncDblBufferHandled(card);
                if(err<0){
                    printf("AI_AsyncDblBufferHandled1 Error: %d\n", err);
                }
            }
            else{
                /*Process buffer 2 data*/
                //printf("Buffer 2 HalfReady, Process the data of Buffer 2\n");
                AI_ContVScale(card, ADC_Range, Buffer2, voltageArray, ReadCount);
                /*
                for(i=0; i<ReadCount; i++){
                    if(fin)
                        fprintf(fin, "%13.9f\n", voltageArray[i]);
                }
                */
                viewidx = 0;
                /*Tell the driver you complete the buffer 2 process*/
                err = AI_AsyncDblBufferHandled(card);
                if(err<0){
                    printf("AI_AsyncDblBufferHandled2 Error: %d\n", err);
                }
            }

            /*
             * This function can check if the overrun occurs. If the
             * function is called, AI_AsyncDblBufferHandled() should
             * be called to let driver to know user buffer is processed
             * completely
             */
            err = AI_AsyncDblBufferOverrun(card, 0, &overrunFlag);
            if(err<0){
                printf("AI_AsyncDblBufferOverrun Error: %d\n", err);
            }
            if(overrunFlag){
                OvrnCnt++;
                //printf("OVERRUN: %d, Lose Some data\n", overrunFlag);
                AI_AsyncDblBufferOverrun(card, 1, &overrunFlag);
            }
            printf("++ Buffer Ready Count: %d, Overrun Count: %d\n", BufRdyCnt, OvrnCnt);
        }
        usleep(1000);
    }while((!Stopped)&&(!kbhit()));

    GetActualRate_9524(card, Group, (F64)ADC_SampRate, &ActualRate);
    printf("\n\nAI Acquisition Done in %f Actual Rate...\n", ActualRate);

    err = AI_AsyncClear(card, &AccessCnt);
    if(err<0){
        printf("AI_AsyncClear Error: %d\n", err);
    }

    if(fin)
        fclose(fin);

    if(!(ConfigCtrl&P9524_AI_BufAutoReset)){
        err = AI_ContBufferReset(card);
        if(err<0){
            printf("AI_ContBufferReset Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
    }

    Release_Card(card);
    printf("Press any key to exit\n");
    getch();
    return 0;
}
