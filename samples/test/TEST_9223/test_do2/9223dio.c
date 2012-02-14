/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2010/01/06                                                       */
/*                                                                            */
/* This sample performs DI read ans DO read/write with polling mode.          */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <string.h>
#include "dask.h"


int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 wPort = 0;
    U16 wLine = 0;
    U16 wDO_WValue = 0;
    U32 dwDO_RValue, dwDI_RValue;
    U8 fLoop_Stop;

    printf("Card Number? ");
    scanf(" %hd", &card_num);

    card = Register_Card(PCI_9223, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    DO_WritePort(card, wPort, 0);

    printf("Line number? ");
    scanf(" %hd", &wLine);
    if(wLine>15){
        printf("Invalid line number, force to line 0\n");
        wLine = 0;
    }

    do{
        /*Write data to DO line*/
        err = DO_WriteLine(card, wPort, wLine, wDO_WValue);
        if(err!=NoError){
            printf("DO_WriteLine Error: %d\n", err);
            break;
        }
        printf("DO Line(%d): %d\n", wLine, wDO_WValue);
        /*Read data from DI port*/
        err = DI_ReadPort(card, 0, &dwDI_RValue);
        if(err<0){
            printf("DI_ReadPort Error: %d\n", err);
            break;
        }
        printf("DI Port: 0x%x\n", dwDI_RValue);
        wDO_WValue = wDO_WValue? 0:1;
        fLoop_Stop = getch();
        if((fLoop_Stop=='q')||(fLoop_Stop=='Q'))
            break;
    }while(1);

    Release_Card(card);
    return 0;
}
