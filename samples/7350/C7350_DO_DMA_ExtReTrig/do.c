/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2009/06/02                                                       */
/*                                                                            */
/* This sample performs continuous DO update with external re-trigger.        */
/* The sample updates 65536 32-bits samples to DIO Port A/B/C/D with 50MHz    */
/* internal sampled clock while the set start trigger is asserted once.       */
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
    U16 DOWaitStatus = P7350_WAIT_EXTTRG;  //Wait External Trigger
    U16 DOClkConfig = P7350_IntSampledCLK; //Internal Sampled Clock
    U16 StartTrigSrc = P7350_AFI_0;        //DO Start Trigger Source
    U32 TrigCount = 4;
    U32 *Buffer;                           //Buffer to be written
    U32 WriteCount = 65536;                //Data Count to be written
    F64 SampleRate = 50000000;             //Update Rate
    U32 vi;
    BOOLEAN Stopped = 0;
    U32 AccessCnt = 0;

    printf("This sample performs continuous DO Update with %ld external\n", TrigCount);
    printf("retrigger (AFI0) at %lf sample rate.\n\n", SampleRate);
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
        0/*TrigConfig*/,
        0/*DO_IPOL*/,
        0/*DO_REQSrc*/,
        0/*DO_ACKSrc*/,
        0/*DO_TRIGSrc*/,
        StartTrigSrc,
        0/*PauseTrigSrc*/,
        0/*SoftTrigOutSrc*/,
        0/*SoftTrigOutLength*/,
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
    //Buffer = (U32 *)VirtualAlloc(NULL, WriteCount*(DOPortWidth/8)*TrigCount, MEM_COMMIT, PAGE_READWRITE);
    Buffer = (U32 *)calloc(WriteCount*TrigCount, (DOPortWidth/8));
    for(vi=0; vi<WriteCount*TrigCount; vi++){
        *(Buffer+vi) = vi;
    }

    /*
     * Perform Continuous DO
     * Note: If the data count is too large, driver may spend some time
     *       to lock memory to avoid memory page-out.
     */
    printf("Press any key to start DO operation with external trigger.\n\n");
    printf("You can press Enter to stop it.\n\n");
    getch();
    err = DO_ContWritePort(card, 0, (void *)Buffer, WriteCount, 1, SampleRate, ASYNCH_OP);
    if(err!=NoError){
        printf("DO_ContReadPort Error: %d\n", err);
        Release_Card(card);
        //VirtualFree(Buffer, 0, MEM_RELEASE);
        free(Buffer);
        exit(1);
    }
    printf("Wait %ld external trigger from AFI0...\n\n", TrigCount);
    do{
        err = DO_AsyncCheck(card, &Stopped, &AccessCnt);
        if(err!=NoError){
            printf("DO_AsyncCheck Error: %d\n", err);
        }
    }while((!kbhit())&&(!Stopped));

    if(!Stopped)
        printf("DO Operation has been stopped manually...\n\n");
    else
        printf("DO Operation Done...\n\n");
    DO_AsyncClear(card, &AccessCnt);

    printf("Press any key to exit...\n\n");
    getch();
    Release_Card(card);
    //VirtualFree(Buffer, 0, MEM_RELEASE);
    free(Buffer);
    return 0;
}
