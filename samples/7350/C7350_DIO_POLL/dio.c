/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2008/12/18                                                       */
/*                                                                            */
/* This sample performs DIO with polling mode.                                */
/* In PCIe-7350, there are 32 programmable DIO channels. Each 8 channels are  */
/* devided into one port that is the DIO configuration unit.                  */
/* You can program the 4 ports to input or output individually.               */
/*----------------------------------------------------------------------------*/
//#include <windows.h>
#include <linux/kernel.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "conio.h"
#include "dask.h"


int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U32 DI_Value0;
    U32 DI_Value1;
    U32 DO_Value = 0;
    U32 DO_Read0;
    U32 DO_Read1;

    printf("This sample perform DIO with polling mode.\n");
    printf("Port A/C are configured as DO ports\n");
    printf("while port B/D are configured as DI ones.\n");
    printf("You can connect port A-C, and B-D to test this sample.\n\n");

    printf("Card Number? ");
    scanf(" %hd", &card_num);

    /*
     * Open and Initialize Device
     */
    card = Register_Card(PCI_7350, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    /*
     * Configure 7350 DIO Port Direction (Input or Output)
     */
    err = DIO_PortConfig(card, P7350_DIO_A, OUTPUT_PORT);
    if(err<0){
        printf("DIO_PortConfig Port_A Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    err = DIO_PortConfig(card, 1, INPUT_PORT);
    if(err<0){
        printf("DIO_PortConfig Port_B Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    err = DIO_PortConfig(card, P7350_DIO_C, OUTPUT_PORT);
    if(err<0){
        printf("DIO_PortConfig Port_C Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    err = DIO_PortConfig(card, P7350_DIO_D, INPUT_PORT);
    if(err<0){
        printf("DIO_PortConfig Port_D Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*
     * Configure DIO Voltage Level
     */
    err = DIO_VoltLevelConfig(card, P7350_PortDIO, VoltLevel_3R3);
    if(err<0){
        printf("DIO_7350_SetVoltLevel Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    else
    {
        printf("DIO_7350_SetVoltLevel ok!");
    }

    /*
     * Read/Write Control
     */
    do{
        clrscr();
        /*Write Port A*/
        err = DO_WritePort(card, P7350_DIO_A, DO_Value);
        if(err<0){
            printf("DO_WritePort Port A Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        /*Write Port C*/
        err = DO_WritePort(card, P7350_DIO_C, 255-DO_Value);
        if(err<0){
            printf("DO_WritePort Port C Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        usleep(1000);
        /*Read Port B*/
        err = DI_ReadPort(card, P7350_DIO_B, &DI_Value0);
        if(err<0){
            printf("DI_ReadPort Port B Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        /*Read Port D*/
        err = DI_ReadPort(card, P7350_DIO_D, &DI_Value1);
        if(err<0){
            printf("DI_ReadPort Port D Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        /*Read Back Port A*/
        err = DO_ReadPort(card, P7350_DIO_A, &DO_Read0);
        if(err<0){
            printf("DO_ReadPort Port A Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        /*Read Back Port C*/
        err = DO_ReadPort(card, P7350_DIO_C, &DO_Read1);
        if(err<0){
            printf("DO_ReadPort Port C Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        printf("-----------------------------------------\n");
        printf(" DO Write Port    Port A    Port C\n");
        printf(" DO Write Data    0x%lx      0x%lx\n\n", DO_Value, 255-DO_Value);
        printf(" DO Read Port     Port A    Port C\n");
        printf(" DO Read Data     0x%lx      0x%lx\n\n", DO_Read0, DO_Read1);
        printf(" DI Read Port     Port B    Port D\n");
        printf(" DI Read Data     0x%lx      0x%lx\n\n", DI_Value0, DI_Value1);
        printf("                 Press Enter to stop...\n");
        printf("-----------------------------------------\n");
        if(++DO_Value==256)
            DO_Value = 0;
        usleep(1000);
    }while(!kbhit());

    printf("Press any key to exit...\n");
    getch();
    Release_Card(card);
    return 0;
}
