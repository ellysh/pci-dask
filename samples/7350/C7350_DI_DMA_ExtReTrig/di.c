/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2009/06/07                                                       */
/*                                                                            */
/* This sample performs continuous DI acquisition with external re-trigger.   */
/* The sample acquires 65536 32-bits samples from DIO Port A/B/C/D with 50MHz */
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
    U16 card_num;
    U16 DIPortWidth = 32;                  //Port Width
    U16 DIMode = P7350_FreeRun;            //DI Mode
    U16 DIWaitStatus = P7350_WAIT_EXTTRG;  //External Digital Trigger
    U16 DIClkConfig = P7350_IntSampledCLK; //Internal Conversion Clock
    U16 StartTrigSrc = P7350_AFI_0;        //DI Start Trigger Source
    U32 TrigCount = 4;                     //Total Trigger Count
    U32 *Buffer;                           //Buffer to be read
    U32 ReadCount = 65536;                 //Data Count to be read
    F64 SampleRate = 50000000;             //Sampling Rate
    U32 vi;
    BOOLEAN Ready;
    BOOLEAN StopFlag = FALSE;
    U16 RdyTrigCnt;
    U32 AccessCnt = 0;
    FILE *fout;
    char fwrite;

    printf("This sample performs continuous DI acquisition with %ld external\n", TrigCount);
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
        0/*TrigConfig*/,
        0/*DI_IPOL*/,
        0/*DI_REQSrc*/,
        0/*DI_ACKSrc*/,
        0/*DI_TRIGSrc*/,
        StartTrigSrc,
        0/*PauseTrigSrc*/,
        0/*SoftTrigOutSrc*/,
        0/*SoftTrigOutLength*/,
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
    //Buffer = (U32 *)VirtualAlloc(NULL, ReadCount*(DIPortWidth/8)*TrigCount, MEM_COMMIT, PAGE_READWRITE);
    //memset(Buffer, '\0', ReadCount*(DIPortWidth/8)*TrigCount);
    Buffer = (U32 *)calloc(ReadCount*TrigCount, (DIPortWidth/8));

    /*
     * Perform Continuous DI
     * Note: If the data count is too large, driver may spend some time
     *       to lock memory to avoid memory page-out.
     */
    printf("Press any key to start DI operation with external trigger.\n");
    printf("You can press Enter to stop it.\n");
    getch();
    err = DI_ContReadPort(card, 0, (void *)Buffer, ReadCount, SampleRate, ASYNCH_OP);
    if(err!=NoError){
        printf("DI_ContReadPort Error: %d\n", err);
        Release_Card(card);
        //VirtualFree(Buffer, 0, MEM_RELEASE);
        free(Buffer);
        exit(1);
    }
    printf("Wait %ld external trigger from AFI0...\n", TrigCount);
    do{
        err = DI_AsyncReTrigNextReady(card, &Ready, &StopFlag, &RdyTrigCnt);
        if(err!=NoError){
            printf("DI_AsyncReTrigNextReady Error: %d\n", err);
        }
        if(Ready)
            printf("Trigger Done: %d\n", RdyTrigCnt);
        if(StopFlag)
            printf("DI Operation Done...\n");
    }while((!kbhit())&&(!StopFlag));
    DI_AsyncClear(card, &AccessCnt);

    if(!StopFlag){
        printf("\nDI Operation has been stopped manually...\n");
    }

    printf("\n\nWrite Data to file? [Y]es or [N]o: ");
    scanf(" %c", &fwrite);
    if((fwrite=='y')||(fwrite=='Y')){
        printf("\n\nWrite Data...");
        if((fout = fopen("di_read.txt", "w"))==NULL)
            printf("fopen error\n");
        else{
            for(vi=0; vi<ReadCount*TrigCount; vi++){
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
