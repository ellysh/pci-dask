/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2008/03/04                                                       */
/*                                                                            */
/* This sample performs SPI operation of 7350.                                */
/* It tests SPI EEPROM read/write operations.                                 */
/* In 7350, AFI0, AFI1, AFI2, and AFI3/4/5(Slave0/1/2) are used for SPI while */
/* performing SPI operation, so AFI0, AFI1, AFI2, and AFI3/4/5(Slave0/1/2)    */
/* will be set to SPI mode automatically if SPI is enabled.                   */
/*                                                                            */
/* Note:                                                                      */
/* Procedures of SPI are depanding on your SPI device,                        */
/* and the sample is developed for ATMEL AT25128A.                            */
/*----------------------------------------------------------------------------*/
//#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "conio.h"
#include "dask.h"

#define SPI_WRTCNT 256


int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 SPI_WriteCmdAddrBits = 24;
    U16 SPI_WriteDataBits = 8;
    U32 SPI_WriteCmdAddr = 0x20000;
    U32 SPI_WriteData[SPI_WRTCNT];
    U16 SPI_ReadCmdAddrBits = 24;
    U16 SPI_ReadDataBits = 8;
    U32 SPI_ReadCmdAddr = 0x30000;
    U32 SPI_ReadData = 0;
    U32 vi = 0;

    printf("This sample tests SPI Read/Write Operations.\n");
    printf("SPI Procedures are depending on the SPI Device.\n");
    printf("It is developed for ATMEL AT25128A.\n");
    printf("It will write a byte-data to and then read from 0 ~ %d Cmd/Addr.\n", SPI_WRTCNT-1);
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    /*Open and initialize device*/
    card = Register_Card(
        PCI_7350, /*Card type*/
        card_num  /*Card number*/
    );
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    /*randon write data*/
    srand(time(NULL));
    for(vi=0; vi<SPI_WRTCNT; vi++)
        SPI_WriteData[vi] = rand()&0xff;

    /*
     * Set AFI voltage level
     * AFI0 ~ AFI3 will be set to SPI mode automatically if SPI is enabled.
     */
    err = DIO_VoltLevelConfig(
        card,          /*Card handle*/
        P7350_PortAFI, /*AFI voltage level to be set*/
        VoltLevel_3R3  /*Voltage level to be set*/
    );
    if(err<0){
        printf("DIO_7350_SetVoltLevel Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*Setup SPI*/
    err = SPI_Setup(
        card,
        SPI_Port_A, /*SPI port to be set*/
        0,          /*SPI configurations*/
        0xff,       /*The parameter is used to set Pre-clock in 7350*/
        0           /*Not used in 7350*/
    );
    if(err<0){
        printf("SPI_Setup Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*
     * Enable SPI
     * AFI0 ~ AFI3 will be set to SPI mode automatically.
     */
    err = SPI_Control(
        card,
        SPI_Port_A,
        SPI_ENABLE, /*Operation item*/
        1           /*Enable opertion item*/
    );
    if(err<0){
        printf("SPI_Control Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    for(vi=0; vi<SPI_WRTCNT; vi++, SPI_WriteCmdAddr++, SPI_ReadCmdAddr++){
        /*Let the specified device (AT25128A) to know write operation will be performed*/
        err = SPI_Write(
            card,
            SPI_Port_A,
            0,    /*Slave Seletion*/
            8,    /*Bits count of cmd/addr*/
            0,    /*Bits count of data*/
            0,    /*Bits of transfer front dummy*/
            0,    /*Bits of transfer tail dummy*/
            0x06, /*cmd/addr to be written*/
            0     /*data to be written*/
        );
        if(err<0){
            printf("SPI_Write Error: %d\n", err);
            SPI_Control(card, SPI_Port_A, SPI_ENABLE, 0);
            Release_Card(card);
            exit(1);
        }

        /*Write*/
        err = SPI_Write(
            card,
            SPI_Port_A,
            0,                    /*Slave Seletion*/
            SPI_WriteCmdAddrBits, /*Bits count of cmd/addr*/
            SPI_WriteDataBits,    /*Bits count of data*/
            0,                    /*Bits of transferred front dummy*/
            0,                    /*Bits of transferred tail dummy*/
            SPI_WriteCmdAddr,     /*cmd/addr to be written*/
            SPI_WriteData[vi]     /*data to be written*/
        );
        if(err<0){
            printf("SPI_Write Error: %d\n", err);
            SPI_Control(card, SPI_Port_A, SPI_ENABLE, 0);
            Release_Card(card);
            exit(1);
        }

        usleep(10000); //Wait for writing SPI EEPROM latency

        /*Read*/
        err = SPI_Read(
            card,
            SPI_Port_A,
            0,                   /*Slave Seletion*/
            SPI_ReadCmdAddrBits, /*Bits count of cmd/addr*/
            SPI_ReadDataBits,    /*Bits count of data*/
            0,                   /*Bits of received front dummy*/
            SPI_ReadCmdAddr,     /*cmd/addr to be read*/
            &SPI_ReadData        /*read data*/
        );
        if(err<0){
            printf("SPI_Read Error: %d\n", err);
            SPI_Control(card, SPI_Port_A, SPI_ENABLE, 0);
            Release_Card(card);
            exit(1);
        }
        if(SPI_ReadData!=SPI_WriteData[vi])
            printf("Read SPI (address: %ld) Failed, Read Data: %ld, Write Data: %ld\n", SPI_ReadCmdAddr&0xffff, SPI_ReadData, SPI_WriteData[vi]);
        else
            printf("Read SPI (address: %ld) Success, Read Data: %ld\n", SPI_ReadCmdAddr&0xffff, SPI_ReadData);
    }

    /*Disable SPI*/
    SPI_Control(
        card,
        SPI_Port_A,
        SPI_ENABLE, /*Opertion item*/
        0           /*Disble opertion item*/
    );

    printf("Press any key to exit...\n");
    getch();
    Release_Card(card);
    return 0;
}
