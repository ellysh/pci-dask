/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2009/06/02                                                       */
/*                                                                            */
/* This sample performs continuous DO update with software trigger.           */
/* The sample updates 65536 32-bits samples to DIO Port A/B/C/D with 50MHz    */
/* internal sampled clock while the software trigger is started.              */
/*----------------------------------------------------------------------------*/
//#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "conio.h"
#include "dask.h"


int main(int argc, char **argv)
{
    I16 card, err;
    I16 card_num;
    U16 DOPortWidth = 32;                  //Port Width
    U16 DOMode = P7350_FreeRun;            //DO Mode
    U16 DOWaitStatus = P7350_WAIT_SOFTTRG; //Software Trigger
    U16 DOClkConfig = P7350_IntSampledCLK; //Internal Sampled Clock
    U16 TrigConfig = P7350_EnSoftTrigOut;  //Enable Software Trigger Out
    U16 SoftTrigOutSrc = P7350_AFI_0;      //Software Trigger Out Source
    U32 SoftTrigOutLength = 10;            //Software Trigger Out Length
    U32 TrigCount = 1;                     //Software Trigger Supports only 1 Trigger
    U32 *Buffer;                           //Buffer to be written
    U32 WriteCount = 65536;                //Data Count to be written
    F64 SampleRate = 50000000;             //Update Rate
    U32 vi;
    BOOLEAN Stopped = 0;
    U32 AccessCnt = 0;

    printf("This sample performs continuous DO Update\n");
    printf("with %lf sample rate.\n\n", SampleRate);
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    /*
     * Open and initialize PCIe-7350
     */
    card = Register_Card(PCI_7350, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    /*
     * Set Voltage Level for DIO Port
     */
    err = DIO_VoltLevelConfig(card, P7350_PortDIO, VoltLevel_3R3);
    if(err!=NoError){
        printf("DIO_VoltLevelConfig Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    err = DIO_VoltLevelConfig(card, P7350_PortAFI, VoltLevel_3R3);
    if(err!=NoError){
        printf("DIO_VoltLevelConfig Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*
     * Set Input or Output for DIO Port
     */
    err = DIO_PortConfig(card, P7350_DIO_A, OUTPUT_PORT);
    if(err!=NoError){
        printf("DIO_PortConfig Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    err = DIO_PortConfig(card, P7350_DIO_B, OUTPUT_PORT);
    if(err!=NoError){
        printf("DIO_PortConfig Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    err = DIO_PortConfig(card, P7350_DIO_C, OUTPUT_PORT);
    if(err!=NoError){
        printf("DIO_PortConfig Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    err = DIO_PortConfig(card, P7350_DIO_D, OUTPUT_PORT);
    if(err!=NoError){
        printf("DIO_PortConfig Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*
     * Set DO Configurations
     */
    err = DO_7350_Config(card, DOPortWidth, DOMode, DOWaitStatus, DOClkConfig);
    if(err!=NoError){
        printf("DO_7350_Config Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*
     * Set DO Trigger and HandShake Configurations
     */
    err = DO_7350_TrigHSConfig(
        card,
        TrigConfig,
        0/*DO_IPOL*/,
        0/*DO_REQSrc*/,
        0/*DO_ACKSrc*/,
        0/*DO_TRIGSrc*/,
        0/*StartTrigSrc*/,
        0/*PauseTrigSrc*/,
        SoftTrigOutSrc,
        SoftTrigOutLength,
        TrigCount
    );
    if(err!=NoError){
        printf("DO_7350_TrigHSConfig Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*
     * allocate buffer to be written -
     * Note: PCIe-7350 DMA engine only supports 8-bytes data and address alignment.
     *       That means -
     *       1. the bytes count of the totally transffed data is the times of 8.
     *       2. the lowest 3 bits of the buffer address is 0.
     */
    /*allocate page-aligned memory to meet 8-bytes alignment*/
    //Buffer = (U32 *)VirtualAlloc(NULL, WriteCount*(DOPortWidth/8), MEM_COMMIT, PAGE_READWRITE);
    Buffer = (U32 *)calloc(WriteCount, (DOPortWidth/8));
    for(vi=0; vi<WriteCount; vi++){
        *(Buffer+vi) = vi;
    }

    /*
     * Perform Continuous DO
     * Note: If the data count is too large, driver may spend some time
     *       to lock memory to avoid memory page-out.
     */
    printf("Press any key to start DO operation.\n");
    getch();
    err = DO_ContWritePort(card, 0, (void *)Buffer, WriteCount, 1, SampleRate, ASYNCH_OP);
    if(err!=NoError){
        printf("DO_ContReadPort Error: %d\n", err);
        Release_Card(card);
        //VirtualFree(Buffer, 0, MEM_RELEASE);
        free(Buffer);
        exit(1);
    }

    /*
     * Set Soft Trigger
     */
    printf("Press any key to generate a software trigger.\n");
    getch();
    printf("generating a software trigger.\n");
    err = DO_7350_SoftTriggerGen(card);
    if(err!=NoError){
        printf("DO_7350_SoftTriggerGen Error: %d\n", err);
        DO_AsyncClear(card, &AccessCnt);
        Release_Card(card);
        //VirtualFree(Buffer, 0, MEM_RELEASE);
        free(Buffer);
        exit(1);
    }
    printf("generated a software trigger.\n");

    do{
        printf("DO_AsyncCheck\n");
        err = DO_AsyncCheck(card, &Stopped, &AccessCnt);
        printf("DO_AsyncCheck ok\n");
        if(err!=NoError){
            printf("DO_AsyncCheck Error: %d\n", err);
        }
    }while((!kbhit())&&(!Stopped));

    if(!Stopped)
        printf("\n\nDO Operation has been stopped manually...\n\n");
    else
        printf("\n\nDO Operation Done...\n\n");
    DO_AsyncClear(card, &AccessCnt);

    printf("Press any key to exit...\n");
    getch();
    Release_Card(card);
    //VirtualFree(Buffer, 0, MEM_RELEASE);
    free(Buffer);
    return 0;
}
