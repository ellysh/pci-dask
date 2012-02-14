/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2009/05/12                                                       */
/*                                                                            */
/* This sample performs continuous DO update. DO operation will start         */
/* immediately.                                                               */
/* PCI-7350 supports the Multi-Buffer mode (Maximum of 64 buffers)to perform  */
/* infinitely continuous DI/DO operations.                                    */
/*----------------------------------------------------------------------------*/
//#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "conio.h"
#include "dask.h"

#define MBufCount 16 //Buffer count to be set

int main(int argc, char **argv)
{
    I16 card, err;
    I16 card_num;
    U16 DOPortWidth = 32;                  //Port width
    U16 DOMode = P7350_FreeRun;            //DO mode
    U16 DOWaitStatus = P7350_WAIT_NO;      //No wait trigger
    U16 DOClkConfig = P7350_IntSampledCLK; //Internal sampled clock
    U32 *Buffer[MBufCount];                //Buffer array to be set
    U16 BufferID[MBufCount];               //Buffer ID returned
    U32 WriteCount = 10000;                //Data count per one buffer to be written
    F64 SampleRate = 10000000;             //Update rate
    U32 vi, vj;
    BOOLEAN NextReady = FALSE;
    U16 ReadyBufferId = 0;
    U32 AccessCnt;

    printf("This sample performs infinitely continuous DO operation to DIO port A/B/C/D\n");
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
     * Set voltage level for DIO port
     */
    err = DIO_VoltLevelConfig(card, P7350_PortDIO, VoltLevel_3R3);
    if(err!=NoError){
        printf("DIO_VoltLevelConfig Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*
     * Set input or output for DIO port
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
     * Set DO configurations
     */
    err = DO_7350_Config(card, DOPortWidth, DOMode, DOWaitStatus, DOClkConfig);
    if(err!=NoError){
        printf("DO_7350_Config Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*
     * Allocate multiple buffers to be written -
     * Note: PCIe-7350 DMA engine only supports 8-bytes data and address alignment.
     *       That means -
     *       1. the bytes count of the totally transffed data is the times of 8.
     *       2. the lowest 3 bits of the buffer address is 0.
     *
     * Allocate page-aligned memory to meet 8-bytes alignment
     */
    for(vi=0; vi<MBufCount; vi++){
        //Buffer[vi] = (U32 *)VirtualAlloc(NULL, WriteCount*(DOPortWidth/8), MEM_COMMIT, PAGE_READWRITE);
        Buffer[vi] = (U32 *)calloc(WriteCount, (DOPortWidth/8));
        /*Assign data to be out to the allocated buffers*/
        for(vj=0; vj<WriteCount; vj++)
            *(Buffer[vi]+vj) = vi*WriteCount+vj;
    }

    /*
     * Setup the allocated buffers -
     * Driver will build the memory descriptor for the specifed buffer
     * to do DMA operation.
     */
    for(vi=0; vi<MBufCount; vi++){
        err = DO_ContMultiBufferSetup(card, Buffer[vi], WriteCount, &BufferID[vi]);
        if(err!=NoError){
            printf("DO_ContMultiBufferSetup %ld Error: %d\n", vi, err);
            Release_Card(card);
            for(vj=0; vj<MBufCount; vj++)
                //VirtualFree(Buffer[vj], 0, MEM_RELEASE);
                free(Buffer[vj]);
            exit(1);
        }
    }

    /*
     * Perform Continuous DO -
     * The buffers set by DO_ContMultiBufferSetup() will be chained to be
     * a ring buffer for DMA engine to infinitely continuous read data
     * from the set buffers. Please refer Chapter 5.2 of PCIS-DASK User
     * Manual for details.
     */
    printf("Press any key to start infinitely continuous DO operation.\n");
    printf("You can press Enter to stop it.\n");
    getch();
    printf("\n\n");
    err = DO_ContMultiBufferStart(card, 0, SampleRate);
    if(err!=NoError){
        printf("DO_ContMultiBufferStart Error: %d\n", err);
        Release_Card(card);
        for(vi=0; vi<MBufCount; vi++)
            //VirtualFree(Buffer[vi], 0, MEM_RELEASE);
            free(Buffer[vi]);
        exit(1);
    }
    do{
        err = DO_AsyncMultiBufferNextReady(card, &NextReady, &ReadyBufferId);
        if(err!=NoError){
            printf("DO_AsyncMultiBufferNextReady Error: %d\n", err);
        }
        if(NextReady){
            printf("Buffer %2d is Ready...\r", ReadyBufferId);
            /*
             * If the NextReady flag is asserted, the data of ready buffers
             * had been read completely to FIFO. You can change the data of
             * ready buffers if necessary.
             */
        }
    }while(!kbhit());
    DO_AsyncClear(card, &AccessCnt);
    printf("DO operation is stopped...\n");

    printf("Press any key to exit...\n");
    getch();
    Release_Card(card);
    for(vi=0; vi<MBufCount; vi++)
        //VirtualFree(Buffer[vi], 0, MEM_RELEASE);
        free(Buffer[vi]);
    return 0;
}
