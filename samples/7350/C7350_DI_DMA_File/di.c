/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2009/06/07                                                       */
/*                                                                            */
/* This sample performs continuous DI acquisition.                            */
/* The sample updates 65536 32-bits samples to DIO Port A/B/C/D with 50MHz    */
/* internal sampled clock while the DI operation is performed.                */
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
    U16 DIClkConfig = P7350_IntSampledCLK; //Internal Conversion Clock
    U32 ReadCount = 65536;                 //Data Count to be read
    F64 SampleRate = 50000000;             //Sampling Rate
    BOOLEAN Stopped;
    U32 AccessCnt;
    U8 FileName[] = "di_read";

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
     * Perform Continuous DI
     * Note: If the data count is too large, driver may spend some time
     *       to lock memory to avoid memory page-out.
     */
    printf("Press any key to start DI operation.\n");
    getch();
    err = DI_ContReadPortToFile(card, 0, FileName, ReadCount, SampleRate, ASYNCH_OP);
    if(err!=NoError){
        printf("DI_ContReadPort Error: %d\n", err);
        Release_Card(card);
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
    printf("Write Data...\n");
    DI_AsyncClear(card, &AccessCnt);
    printf("\rWrite Data Done... \n");

    printf("Press any key to exit...\n");
    getch();
    Release_Card(card);
    return 0;
}
