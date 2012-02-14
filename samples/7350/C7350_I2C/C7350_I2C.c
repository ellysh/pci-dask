/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2009/03/04                                                       */
/*                                                                            */
/* This sample performs I2C operation of 7350.                                */
/* It tests I2C EEPROM read/write operations.                                 */
/* In 7350, AFI0 ~ AFI1 are used for I2C while performing I2C operation,      */
/* so AFI0 ~ AFI1 will be set to I2C mode automatically if I2C is enabled.    */
/*----------------------------------------------------------------------------*/
//#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "conio.h"
#include "dask.h"

#define I2C_WRTCNT 256


int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U32 I2C_ReadCmdAddr;
    U32 I2C_ReadData;
    U32 I2C_WriteCmdAddr;
    U32 I2C_WriteData;
    U16 I2C_SlaveAddr;
    U32 vi = 0;
    U16 PatType;

    printf("This sample tests I2C Read/Write Operations.\n");
    printf("It will write a byte-data to and then read from 0 ~ %d Cmd/Addr.\n", I2C_WRTCNT-1);
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    printf("I2C Slave Address? ");
    scanf(" %hd", &I2C_SlaveAddr);

    printf("Increasing or Decreasing Pattern? [0]Increasing [1]Decreasing ");
    scanf(" %hd", &PatType);
    PatType &= 0x1;

    /*Open and initialize device*/
    card = Register_Card(
        PCI_7350, /*Card type*/
        card_num  /*Card number*/
    );
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    /*
     * Set AFI voltage level
     * AFI0/AFI1 will be set to I2C mode automatically if I2C is enabled.
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

    /*Setup I2C*/
    err = I2C_Setup(
        card,
        I2C_Port_A, /*I2C port to be set*/
        0,          /*Not used in 7350*/
        0xff,       /*The parameter is used to set Pre-clock in 7350*/
        0           /*Not used in 7350*/
    );
    if(err<0){
        printf("I2C_Setup Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*
     * Enable I2C
     * AFI0/AFI1 will be set to I2C mode automatically.
     */
    err = I2C_Control(
        card,
        I2C_Port_A,
        I2C_ENABLE, /*Operation item*/
        1           /*Enable opertion item*/
    );
    if(err<0){
        printf("I2C_Control Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    do{
        /*Write*/
        I2C_WriteCmdAddr = vi&0xff;
        I2C_WriteData = (!PatType)? I2C_WriteCmdAddr:255-I2C_WriteCmdAddr;
        err = I2C_Write(
            card,
            I2C_Port_A,
            I2C_SlaveAddr,    /*Slave device address*/
            1,                /*Address byte count to be written*/
            1,                /*Data byte count to be written*/
            I2C_WriteCmdAddr, /*Cmd/Addr to be written*/
            I2C_WriteData     /*Data to be written*/
        );
        if(err<0){
            printf("I2C_Write Error: %d\n", err);
            I2C_Control(card, I2C_Port_A, I2C_ENABLE, 0);
            Release_Card(card);
            exit(1);
        }

        usleep(10000); //Wait for writing I2C EEPROM latency

        /*Read*/
        I2C_ReadCmdAddr = I2C_WriteCmdAddr;
        err = I2C_Read(
            card,
            I2C_Port_A,
            I2C_SlaveAddr,   /*Slave device address*/
            1,               /*Address byte count to be read*/
            1,               /*Data byte count to be read*/
            I2C_ReadCmdAddr, /*Cmd/Addr to be read*/
            &I2C_ReadData    /*read Data*/
        );
        if(err<0){
            printf("I2C_Read Error: %d\n", err);
            I2C_Control(card, I2C_Port_A, I2C_ENABLE, 0);
            Release_Card(card);
            exit(1);
        }
        printf("Read Addr(%ld): %ld\n", I2C_ReadCmdAddr, I2C_ReadData);
        vi++;
    }while((vi<256)&&(!kbhit()));

    /*Disable I2C*/
    err = I2C_Control(
        card,
        I2C_Port_A,
        I2C_ENABLE, /*Opertion item*/
        0           /*Disble opertion item*/
    );
    if(err<0){
        printf("I2C_Control Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    else
        printf("I2C disabled\n");

    printf("Press any key to exit...\n");
    getch();
    Release_Card(card);
    return 0;
}
