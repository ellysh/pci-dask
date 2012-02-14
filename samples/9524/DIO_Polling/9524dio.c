/*----------------------------------------------------------------------------*/
/* Date: 2009/08/19                                                           */
/* Company: ADLINK Technology Inc.                                            */
/*                                                                            */
/* This sample outputs digital data to DO port, and read data from DI port    */
/* and DI port.                                                               */
/* You can connect DO port and DI port.                                       */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

int main(int argc, char **argv)
{
    I16 card;
    I16 err;
    U16 card_num;
    U32 dwDOWriteValue=0;
    U32 dwDOReadValue;
    U32 dwDIReadValue;

    printf("This sample writes data to DO port, and reads data from DI port.\n");
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    card = Register_Card(PCI_9524, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    do{
        clrscr();

        printf("/*--------------------------------*/\n");

        /*Write DO Port*/
        err = DO_WritePort(card, 0, dwDOWriteValue);
        if(err<0){
            printf("DO_WritePort Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        printf("\n    Write DO port value: %d\n", dwDOWriteValue);
        usleep(1000);

        /*Read DO Port*/
        err = DO_ReadPort(card, 0, &dwDOReadValue);
        if(err<0){
            printf("DO_ReadPort Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        printf("\n    Read DO port value: %d\n", dwDOReadValue);

        /*Read DI Port*/
        err = DI_ReadPort(card, 0, &dwDIReadValue);
        if(err<0){
            printf("DI_ReadPort Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        printf("\n    Read DI port value: %d\n", dwDIReadValue);

        printf("\n\t\tpress Enter to stop\n");
        printf("/*--------------------------------*/\n");

        if((++dwDOWriteValue)>0xff)
            dwDOWriteValue = 0;
        usleep(100000);
    }while(!kbhit());

    Release_Card(card);
    return 0;
}
