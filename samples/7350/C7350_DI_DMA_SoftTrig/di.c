/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2009/06/05                                                       */
/*                                                                            */
/* This sample performs continuous DI acquisition with software trigger.      */
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
    U16 card_num;
    U16 DIPortWidth = 32;                  //Port Width
    U16 DIMode = P7350_FreeRun;            //DI Mode
    U16 DIWaitStatus = P7350_WAIT_SOFTTRG; //Software Trigger
    U16 DIClkConfig = P7350_IntSampledCLK; //Internal Conversion Clock
    U16 TrigConfig = P7350_EnSoftTrigOut;  //Enable Software Trigger Out
    U16 SoftTrigOutSrc = P7350_AFI_0;      //Software Trigger Out Source
    U32 SoftTrigOutLength = 10;            //Software Trigger Out Length
    U32 TrigCount = 1;                     //Software Trigger Supports only 1 Trigger
    U32 *Buffer;                           //Buffer to be read
    U32 ReadCount = 65536;                 //Data Count to be read
    F64 SampleRate = 50000000;             //Sampling Rate
    U32 vi;
    BOOLEAN Stopped = 0;
    U32 AccessCnt = 0;
    FILE *fout;
    char fwrite;

    printf("This sample performs continuous DI acquisition\n");
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
     * Set DI Trigger and HandShake Configurations
     */
    err = DI_7350_TrigHSConfig(
        card,
        TrigConfig,
        0/*DI_IPOL*/,
        0/*DI_REQSrc*/,
        0/*DI_ACKSrc*/,
        0/*DI_TRIGSrc*/,
        0/*StartTrigSrc*/,
        0/*PauseTrigSrc*/,
        SoftTrigOutSrc,
        SoftTrigOutLength,
        TrigCount
    );
    if(err!=NoError){
        printf("DI_7350_TrigHSConfig Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*
     * allocate buffer to be read -
     * Note: PCIe-7350 DMA engine only supports 8-bytes data and address alignment.
     *       That means -
     *       1. the bytes count of the totally transffed data is the times of 8.
     *       2. the lowest 3 bits of the buffer address is 0.
     */
    /*allocate page-aligned memory to meet 8-bytes alignment*/
    //Buffer = (U32 *)VirtualAlloc(NULL, ReadCount*(DIPortWidth/8), MEM_COMMIT, PAGE_READWRITE);
    //memset(Buffer, '\0', ReadCount*(DIPortWidth/8));
    Buffer = (U32 *)calloc(ReadCount, (DIPortWidth/8));

    /*
     * Perform Continuous DI
     * Note: If the data count is too large, driver may spend some time
     *       to lock memory to avoid memory page-out.
     */
    printf("Press any key to start DI operation.\n");
    getch();
    err = DI_ContReadPort(card, 0, (void *)Buffer, ReadCount, SampleRate, ASYNCH_OP);
    if(err!=NoError){
        printf("DI_ContReadPort Error: %d\n", err);
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
    err = DI_7350_SoftTriggerGen(card);
    if(err!=NoError){
        printf("DI_7350_SoftTriggerGen Error: %d\n", err);
        DI_AsyncClear(card, &AccessCnt);
        Release_Card(card);
        //VirtualFree(Buffer, 0, MEM_RELEASE);
        free(Buffer);
        exit(1);
    }

    do{
        err = DI_AsyncCheck(card, &Stopped, &AccessCnt);
        if(err!=NoError){
            printf("DI_AsyncCheck Error: %d\n", err);
        }
    }while((!kbhit())&&(!Stopped));

    if(!Stopped)
        printf("DI Operation has been stopped manually...\n");
    else
        printf("DI Operation Done...\n");
    DI_AsyncClear(card, &AccessCnt);

    printf("Write Data to file? [Y]es or [N]o: \n");
    scanf(" %c", &fwrite);
    if((fwrite=='y')||(fwrite=='Y')){
        printf("\n\nWrite Data...");
        if((fout = fopen("di_read.txt", "w"))==NULL)
            printf("fopen error\n");
        else{
            for(vi=0; vi<ReadCount; vi++){
                fprintf(fout, "%ld\n", *(Buffer+vi));
            }
            fclose(fout);
        }
        printf("\rWrite Data Done...\n\n");
    }

    printf("Press any key to exit...\n");
    getch();
    Release_Card(card);
    //VirtualFree(Buffer, 0, MEM_RELEASE);
    free(Buffer);
    return 0;
}
