/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2009/06/07                                                       */
/*                                                                            */
/* This sample performs continuous DI acquisition with external sampled clock.*/
/* The sample acquires 65536 32-bits samples from DIO Port A/B/C/D with       */
/* external sampled clock while the DI is started.                            */
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
    U16 DIWaitStatus = P7350_WAIT_NO;      //No Wait Trigger
    U16 DIClkConfig = P7350_ExtSampledCLK; //External Sampled Clock
    U16 DICLK_Src = P7350_AFI_7;
    U32 *Buffer;                           //Buffer to be read
    U32 ReadCount = 65536;                 //Data Count to be read
    U32 vi;
    BOOLEAN Stopped;
    U32 AccessCnt;
    FILE *fout;
    char fwrite;

    printf("This sample performs continuous DI acquisition\n");
    printf("with external sample clock (AFI7).\n\n");
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
     * Set DI Sampled Clock
     */
    err = DI_7350_ExtSampCLKConfig(
        card,
        DICLK_Src,
        0/*DICLK_DDAMode*/,
        0/*DICLK_DPAMode*/,
        0/*DICLK_DDAVlaue*/,
        0/*DICLK_DPAVlaue*/
    );
    if(err!=NoError){
        printf("DI_7350_ExtSampCLKConfig Error: %d\n", err);
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
    Buffer = (U32 *)calloc(ReadCount, (DIPortWidth/8));
    //memset(Buffer, '\0', ReadCount*(DIPortWidth/8));

    /*
     * Perform Continuous DI
     * Note: If the data count is too large, driver may spend some time
     *       to lock memory to avoid memory page-out.
     */
    printf("Press any key to start DI operation.\n");
    printf("DI data will be acquired with the set externel sampled clock (AFI7).\n");
    printf("You can press Enter to stop it.\n");
    getch();
    err = DI_ContReadPort(card, 0, (void *)Buffer, ReadCount, 0, ASYNCH_OP);
    if(err!=NoError){
        printf("DI_ContReadPort Error: %d\n", err);
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
        printf("\n\nDI Operation has been stopped manually...\n\n");
    else
        printf("\n\nDI Operation Done...\n\n");
    DI_AsyncClear(card, &AccessCnt);

    printf("\n\nWrite Data to file? [Y]es or [N]o: ");
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
    free(Buffer);
    //VirtualFree(Buffer, 0, MEM_RELEASE);
    return 0;
}
