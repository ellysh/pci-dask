#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 Port = 0;
    U32 out_data = 0;
    U32 rb_data;
    U32 read_data;
    int i = 0;

    printf("This sample outputs digital value to DO Port, \n");
    printf("and reads DI Port. (Connect DI and DO Port)\n\n");
    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if((card=Register_Card (PCI_9221, card_num)) < 0) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

    do{
        clrscr();

        //Write Data to DO Port
        err =  DO_WritePort(card, Port, out_data);
        if(err<0){
            printf("DO Write Port Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        printf("DO Write Data: 0x%x\n", (unsigned int)out_data);

        //Read Back Data from DO Port
        err =  DO_ReadPort(card, Port, &rb_data);
        if(err<0){
            printf("DO Read Port Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        printf("DO Read Back: 0x%x\n", (unsigned int)rb_data);

        //Read Data from DI Port
        err =  DI_ReadPort(card, Port, &read_data);
        if(err<0){
            printf("DI Read Port Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        printf("DI Read Data: 0x%x\n", (unsigned int)read_data);

        out_data = (++out_data)%16;
        printf("Press Enter to exit...\n");
        usleep(200000);
    }while(!kbhit());

    Release_Card(card);
    return 0;
}
