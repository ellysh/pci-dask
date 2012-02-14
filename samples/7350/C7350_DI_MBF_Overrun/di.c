/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2009/06/05                                                       */
/*                                                                            */
/* This sample performs continuous DI acquisition. DI operation will start    */
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
    U16 card_num;
    U16 DIPortWidth = 32;                  //Port Width
    U16 DIMode = P7350_FreeRun;            //DI Mode
    U16 DIWaitStatus = P7350_WAIT_NO;      //No Wait Trigger
    U16 DIClkConfig = P7350_IntSampledCLK; //Internal Conversion Clock
    U32 *Buffer[MBufCount];                //Buffer to be written
    U16 BufferID[MBufCount];               //Buffer ID returned
    U32 ReadCount = 10000;                 //Data Count to be read
    F64 SampleRate = 10000000;             //Sampling Rate
    U32 vi, vj;
    BOOLEAN NextReady = FALSE;
    U16 ReadyBufferId = 0;
    U32 AccessCnt;
    U32 BufRdyCnt = 0;
    U32 OverrunCnt = 0;
    U16 PreDIReadyBufferId = 0;
    U16 bufcnt = 0;
    U16 overrunFlag;

    printf("This sample performs infinitely continuous DI acquisition to DIO port A/B/C/D\n");
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

    /*
     * Set Input or Output for DIO Port
     */
    err = DIO_PortConfig(card, P7350_DIO_A, INPUT_PORT);
    if(err!=NoError){
        printf("DIO_PortConfig Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    err = DIO_PortConfig(card, P7350_DIO_B, INPUT_PORT);
    if(err!=NoError){
        printf("DIO_PortConfig Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    err = DIO_PortConfig(card, P7350_DIO_C, INPUT_PORT);
    if(err!=NoError){
        printf("DIO_PortConfig Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    err = DIO_PortConfig(card, P7350_DIO_D, INPUT_PORT);
    if(err!=NoError){
        printf("DIO_PortConfig Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*
     * Set DI Configurations
     */
    err = DI_7350_Config(card, DIPortWidth, DIMode, DIWaitStatus, DIClkConfig);
    if(err!=NoError){
        printf("DI_7350_Config Error: %d\n", err);
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
        //Buffer[vi] = (U32 *)VirtualAlloc(NULL, ReadCount*(DIPortWidth/8), MEM_COMMIT, PAGE_READWRITE);
        //memset(Buffer[vi], '\0', ReadCount*(DIPortWidth/8));
        Buffer[vi] = (U32 *)calloc(ReadCount, (DIPortWidth/8));
    }

    /*
     * Setup the allocated buffers -
     * Driver will build the memory descriptor for the specifed buffer
     * to do DMA operation.
     */
    for(vi=0; vi<MBufCount; vi++){
        err = DI_ContMultiBufferSetup(card, Buffer[vi], ReadCount, &BufferID[vi]);
        if(err!=NoError){
            printf("DI_ContMultiBufferSetup %ld Error: %d\n", vi, err);
            Release_Card(card);
            for(vj=0; vj<MBufCount; vj++)
                //VirtualFree(Buffer[vj], 0, MEM_RELEASE);
                free(Buffer[vj]);
            exit(1);
        }
    }

    /*
     * Perform Continuous DI -
     * The buffers set by DI_ContMultiBufferSetup() will be chained to be
     * a ring buffer for DMA engine to infinitely continuous write data
     * to the set buffers. Please refer Chapter 5.2 of PCIS-DASK User
     * Manual for details.
     */
    printf("Press any key to start infinitely continuous DI operation.\n");
    printf("You can press Enter to stop it.\n");
    getch();
    printf("\n");
    err = DI_ContMultiBufferStart(card, 0, SampleRate);
    if(err!=NoError){
        printf("DI_ContMultiBufferStart Error: %d\n", err);
        Release_Card(card);
        for(vi=0; vi<MBufCount; vi++)
            //VirtualFree(Buffer[vi], 0, MEM_RELEASE);
            free(Buffer[vi]);
        exit(1);
    }
    do{
        err = DI_AsyncMultiBufferNextReady(card, &NextReady, &ReadyBufferId);
        if(err!=NoError){
            printf("DI_AsyncMultiBufferNextReady Error: %d\n", err);
        }
        if(NextReady){
            /*
             * If the NextReady flag is asserted, the data of ready buffers
             * had been read completely. You can process the data of ready
             * buffers if necessary.
             *
             * Process the data of the ready buffers in here
             */

            /*
             * Tell driver that the ready buffers are handled completely
             */
            if(ReadyBufferId<PreDIReadyBufferId){
                bufcnt = MBufCount-PreDIReadyBufferId+ReadyBufferId+1;
                err = DI_AsyncMultiBuffersHandled(
                    card,
                    (U16)(MBufCount-PreDIReadyBufferId),
                    &BufferID[PreDIReadyBufferId]
                );
                if(err!=NoError)
                    printf("DI_AsyncMultiBuffersHandled1 Error: %d\n", err);
                err = DI_AsyncMultiBuffersHandled(
                    card,
                    (U16)(ReadyBufferId+1),
                    &BufferID[0]
                );
                if(err!=NoError)
                    printf("DI_AsyncMultiBuffersHandled2 Error: %d\n", err);
            }
            else{
                bufcnt = ReadyBufferId-PreDIReadyBufferId+1;
                err = DI_AsyncMultiBuffersHandled(
                    card,
                    bufcnt,
                    &BufferID[PreDIReadyBufferId]
                );
                if(err!=NoError)
                    printf("DI_AsyncMultiBuffersHandled Error: %d\n", err);
            }
            /*
             * Check whether the DI is overrun
             */
            err = DI_AsyncDblBufferOverrun(card, 0, &overrunFlag);
            if(err!=NoError){
                printf("DI_AsyncDblBufferOverrun Error: %d\n", err);
            }
            if(overrunFlag){
                //printf("DI Over Run...\n");
                OverrunCnt++;
                DI_AsyncDblBufferOverrun(card, 1, &overrunFlag);
            }
            PreDIReadyBufferId = ReadyBufferId+1;
            BufRdyCnt++;
            printf("Buffer Ready Count: %ld, Overrun Count: %ld\r", BufRdyCnt, OverrunCnt);
        }
    }while(!kbhit());
    DI_AsyncClear(card, &AccessCnt);
    printf("DI Operation Done...\n");

    printf("Press any key to exit...\n");
    getch();
    Release_Card(card);
    for(vi=0; vi<MBufCount; vi++)
        //VirtualFree(Buffer[vi], 0, MEM_RELEASE);
        free(Buffer[vi]);
    return 0;
}
