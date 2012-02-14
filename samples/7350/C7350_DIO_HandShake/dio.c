/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2009/06/09                                                       */
/*                                                                            */
/* This sample performs DO and DI Handshake operation with DI_TRIG.           */
/*                                                                            */
/* DO REQ Source (AFI2) <-------> DI REQ Source (AFI3)                        */
/* DO ACK Source (AFI4) <-------> DI ACK Source (AFI5)                        */
/* DO Software Trigger Out (AFI0) -------> DI TRIG Source (AFI1)              */
/* 16 Bits DO Data (DIO Port A/C) -------> 16 Bits DI Data (DIO Port B/D)     */
/*----------------------------------------------------------------------------*/
//#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "conio.h"
#include "dask.h"


int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 DOPortWidth = 16;                   //Port Width
    U16 DOMode = P7350_HandShake;           //DO Mode
    U16 DOWaitStatus = P7350_WAIT_SOFTTRG;  //No Wait Trigger
    U16 DIPortWidth = 16;                   //Port Width
    U16 DIMode = P7350_HandShake;           //DI Mode
    U16 DIWaitStatus = P7350_WAIT_EXTTRG;   //No Wait Trigger
    U16 DOTrigConfig = P7350_EnSoftTrigOut; //Enable Soft Trigger Out
    U16 DO_REQSrc = P7350_AFI_2;            //DO REQ Source
    U16 DO_ACKSrc = P7350_AFI_4;            //DO ACK Source
    U16 DOSoftTrigOutSrc = P7350_AFI_0;     //Soft Trigger Out Source
    U32 DOSoftTrigOutLength = 10;           //Soft Trigger Out Length
    U32 DOTrigCount = 1;                    //Trigger Count
    U16 DI_REQSrc = P7350_AFI_3;            //DI REQ Source
    U16 DI_ACKSrc = P7350_AFI_5;            //DI ACK Source
    U16 DI_TRIGSrc = P7350_AFI_1;           //DI TRIG Source
    U32 DITrigCount = 1;                    //Trigger Count
    U16 *DOBuffer;                          //Buffer to be written
    U16 *DIBuffer;                          //Buffer to be read
    U32 WriteCount = 65536;                 //Data Count to be written
    U32 ReadCount = 65536;                  //Data Count to be read
    U32 vi;
    BOOLEAN DIStopped = 0;
    U32 DIAccessCnt = 0;
    BOOLEAN DOStopped = 0;
    U32 DOAccessCnt = 0;
    //BOOLEAN fwrt;
    char fwrt;
    FILE *fout;

    printf("The sample performs DIO Handshake operation.\n");
    printf("DIO port A/C are configured as the DO port and\n");
    printf("DIO port B/D are configured as the DI port.\n");
    printf("AFI2 and AFI4 are set to be as DO_REQ and DO_ACK\n");
    printf("while AFI3 and AFI5 are set to be as DI_REQ and DI_ACK.\n");
    printf("And DO is set to enable software trigger out (AFI0) to\n");
    printf("be as the DI_TRIG source (AFI1).\n");
    printf("Please connect    AFI0  <---> AFI1\n");
    printf("                  AFI2  <---> AFI3\n");
    printf("                  AFI4  <---> AFI5\n");
    printf("                  PortA <---> PortB\n");
    printf("                  PortC <---> PortD\n");

    printf("\nCard Number? ");
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
    err = DIO_PortConfig(card, P7350_DIO_B, INPUT_PORT);
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
    err = DIO_PortConfig(card, P7350_DIO_D, INPUT_PORT);
    if(err!=NoError){
        printf("DIO_PortConfig Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*
     * Set DO Configurations
     */
    err = DO_7350_Config(card, DOPortWidth, DOMode, DOWaitStatus, 0/*DOClkConfig*/);
    if(err!=NoError){
        printf("DO_7350_Config Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    /*
     * Set DI Configurations
     */
    err = DI_7350_Config(card, DIPortWidth, DIMode, DIWaitStatus, 0/*DIClkConfig*/);
    if(err!=NoError){
        printf("DI_7350_Config Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*
     * Set DO Trigger and HandShake Configurations
     */
    err = DO_7350_TrigHSConfig(
        card,
        DOTrigConfig,
        0/*DO_IPOL*/,
        DO_REQSrc,
        DO_ACKSrc,
        0/*DO_TRIGSrc*/,
        0/*StartTrigSrc*/,
        0/*PauseTrigSrc*/,
        DOSoftTrigOutSrc,
        DOSoftTrigOutLength,
        DOTrigCount
    );
    if(err!=NoError){
        printf("DO_7350_TrigHSConfig Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    /*
     * Set DI Trigger and HandShake Configurations
     */
    err = DI_7350_TrigHSConfig(
        card,
        0/*TrigConfig*/,
        0/*DI_IPOL*/,
        DI_REQSrc,
        DI_ACKSrc,
        DI_TRIGSrc,
        0/*DIStartTrigSrc*/,
        0/*PauseTrigSrc*/,
        0/*SoftTrigOutSrc*/,
        0/*SoftTrigOutLength*/,
        DITrigCount
    );
    if(err!=NoError){
        printf("DI_7350_TrigHSConfig Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*
     * allocate buffer to be read/written -
     * Note: PCIe-7350 DMA engine only supports 8-bytes data and address alignment.
     *       That means -
     *       1. the bytes count of the totally transffed data is the times of 8.
     *       2. the lowest 3 bits of the buffer address is 0.
     */
    /*allocate page-aligned memory to meet 8-bytes alignment*/
    //DOBuffer = (U16 *)VirtualAlloc(NULL, WriteCount*(DOPortWidth/8), MEM_COMMIT, PAGE_READWRITE);
    DOBuffer = (U16 *)calloc(WriteCount, (DOPortWidth/8));
    for(vi=0; vi<WriteCount; vi++){
        *(DOBuffer+vi) = (U16)(vi%65536);
    }
    //DIBuffer = (U16 *)VirtualAlloc(NULL, ReadCount*(DIPortWidth/8), MEM_COMMIT, PAGE_READWRITE);
    //memset(DIBuffer, '\0', ReadCount*(DIPortWidth/8));
    DIBuffer = (U16 *)calloc(ReadCount, (DIPortWidth/8));

    /*
     * Perform Continuous DI
     * Note: If the data count is too large, driver may spend some time
     *       to lock memory to avoid memory page-out.
     */
    err = DI_ContReadPort(card, 0, (void *)DIBuffer, ReadCount, 0/*DISampleRate*/, ASYNCH_OP);
    if(err!=NoError){
        printf("DI_ContReadPort Error: %d\n", err);
        Release_Card(card);
        //VirtualFree(DOBuffer, 0, MEM_RELEASE);
        //VirtualFree(DIBuffer, 0, MEM_RELEASE);
        free(DOBuffer);
        free(DIBuffer);
        exit(1);
    }

    /*
     * Perform Continuous DO
     * Note: If the data count is too large, driver may spend some time
     *       to lock memory to avoid memory page-out.
     */
    err = DO_ContWritePort(card, 0, (void *)DOBuffer, WriteCount, 1, 0/*DOSampleRate*/, ASYNCH_OP);
    if(err!=NoError){
        printf("DO_ContWritePort Error: %d\n", err);
        Release_Card(card);
        //VirtualFree(DOBuffer, 0, MEM_RELEASE);
        //VirtualFree(DIBuffer, 0, MEM_RELEASE);
        free(DOBuffer);
        free(DIBuffer);
        exit(1);
    }

    /*
     * Set Soft Trigger
     */
    printf("Press any key to generate DO soft trigger...\n\n");
    getch();
    err = DO_7350_SoftTriggerGen(card);
    if(err!=NoError){
        printf("DO_7350_SoftTriggerGen Error: %d\n", err);
        DI_AsyncClear(card, &DIAccessCnt);
        DO_AsyncClear(card, &DOAccessCnt);
        Release_Card(card);
        //VirtualFree(DOBuffer, 0, MEM_RELEASE);
        //VirtualFree(DIBuffer, 0, MEM_RELEASE);
        free(DOBuffer);
        free(DIBuffer);
        exit(1);
    }

    do{
        err = DO_AsyncCheck(card, &DOStopped, &DOAccessCnt);
        if(err!=NoError){
            printf("DO_AsyncCheck Error: %d\n", err);
        }
    }while((!kbhit())&&(!DOStopped));
    DO_AsyncClear(card, &DOAccessCnt);
    printf("DO Operation Done...\n\n");

    do{
        err = DI_AsyncCheck(card, &DIStopped, &DIAccessCnt);
        if(err!=NoError){
            printf("DI_AsyncCheck Error: %d\n", err);
        }
    }while((!kbhit())&&(!DIStopped));
    DI_AsyncClear(card, &DIAccessCnt);
    printf("DI Operation Done...\n\n");

    printf("Write Data to file? [Y]es or [N]o:\n\n ");
    scanf(" %c", &fwrt);
    if((fwrt=='y')||(fwrt=='Y')){
        printf("\n\nWrite Data...");
        if((fout = fopen("di_read.txt", "w"))==NULL)
            printf("fopen error\n");
        else{
            for(vi=0; vi<ReadCount; vi++){
                fprintf(fout, "%d\n", *(DIBuffer+vi));
            }
            fclose(fout);
        }
        printf("\rWrite Data Done...\n\n");
    }

    printf("Press any key to exit...\n");
    getch();
    Release_Card(card);
    //VirtualFree(DOBuffer, 0, MEM_RELEASE);
    //VirtualFree(DIBuffer, 0, MEM_RELEASE);
    free(DOBuffer);
    free(DIBuffer);
    return 0;
}
