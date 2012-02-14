#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include "dask.h"


int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U32 BaseAddr;
    U32 BaseAddr2;
    U32 LcrAddr;

    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if((card = Register_Card(PCI_9222, card_num))<0){
        printf("Register_Card Error: %d", card);
        exit(1);
    }
    printf("Register Card Success: %d\n", card);

    err = GetBaseAddr(card, &BaseAddr, &BaseAddr2);
    if(err!=NoError){
        printf("GetBaseAddr Error: %d\n", err);
        goto err_ret;
    }
    printf("Base Address1: 0x%x\n", (unsigned int)BaseAddr);
    printf("Base Address2: 0x%x\n", (unsigned int)BaseAddr2);

    err = GetLCRAddr(card, &LcrAddr);
    if(err!=NoError){
        printf("GetLCRAddr Error: %d\n", err);
        goto err_ret;
    }
    printf("LCR Address: 0x%x\n", (unsigned int)LcrAddr);

err_ret:
    Release_Card(card);
    return 0;
}
