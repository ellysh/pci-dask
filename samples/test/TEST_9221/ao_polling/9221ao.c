#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    F32 out_data;
    char qflag;
    
    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if((card=Register_Card (PCI_9221, card_num)) < 0) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

    do{
        clrscr();
	
        printf("Input AO Value to be written to CH0:");
        scanf(" %f", &out_data);
        err = AO_VWriteChannel(card, 0, (F64)out_data);
	if(err < 0){
            printf("AO Write Ch0 Error: %d\n", err);  
	    exit(1);
	}
        printf("Input AO Value to be written to CH1:");
        scanf(" %f", &out_data);
        err = AO_VWriteChannel(card, 1, (F64)out_data);
	if(err < 0){
            printf("AO Write Ch1 Error: %d\n", err);  
	    exit(1);
	}

        printf("\nPress any key to continue, and 'q' to exit...\n");
        qflag = getch();
    }while(qflag!='q');
    
    err = Release_Card(card);
    if(err < 0){
        printf("Release_Card error=%d", err);
        exit(1);
    }
 
    return 0;
}
