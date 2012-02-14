#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U32 out_data;
    U32 rb_data;
    U32 read_data;
    int i = 0;
    
    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if((card=Register_Card (PCI_9221, card_num)) < 0) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

    do{
        clrscr();
	if(i==16)
	    i=0;
        out_data = i++;
        //Write Data to DO Port
	err =  DO_WritePort(card, 0, out_data);
        if(err < 0){
            printf("DO Write Port Error: %d\n", err);
	    exit(1);
        }
        printf("DO Write Data: 0x%x\n", (unsigned int)out_data);
        //Read Back Data from DO Port
	err =  DO_ReadPort(card, 0, &rb_data);
        if(err < 0){
            printf("DO Read Port Error: %d\n", err);
	    exit(1);
        }
        printf("DO Read Back: 0x%x\n", (unsigned int)rb_data);
        //Read Data from DI Port
        err =  DI_ReadPort(card, 0, &read_data);
        if(err < 0){
            printf("DI Read Port Error: %d\n", err);
	    exit(1);
        }
        printf("DI Read Data: 0x%x\n", (unsigned int)read_data);
        printf("Press any key to exit...\n");
	usleep(200000);

    }while(!kb_hit());
    
    err = Release_Card(card);
    if(err < 0){
        printf("Release_Card error=%d", err);
        exit(1);
    }
 
    return 0;
}
