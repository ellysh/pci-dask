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
    U32 vi = 0;

    printf("Please input a card number: ");
    scanf(" %hd", &card_num);
do{
printf("*** %d ***\n", vi);
    if((card = Register_Card(PCI_7350, card_num))<0){
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
sleep(1);
}while(((++vi)<100)&&(!kb_hit()));
    return 0;
}

