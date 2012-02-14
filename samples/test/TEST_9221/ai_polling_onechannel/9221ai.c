#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include "dask.h"

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 acq_value;
    F64 v_acq_value;
    
    
    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if((card=Register_Card (PCI_9221, card_num)) < 0) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

    do{
        clrscr();
        usleep(100000);
        //err = AI_ReadChannel(card, 0, AD_B_5_V, &acq_value);
        err = AI_VReadChannel(card, 0, AD_B_5_V, &v_acq_value);
        if(err < 0){
            printf("AI Read Ch0 Error: %d\n", err);  
           exit(1);
        }
        //printf("\nAI CH0 ACQ Value: 0x%x\n", acq_value);
        printf("\nAI CH0 ACQ Value: %f\n", (F32)v_acq_value);
        printf("\n\n     Press any key to exit...\n");
    }while(!kb_hit());
    
    err = Release_Card(card);
    if(err < 0){
        printf("Release_Card error=%d", err);
        exit(1);
    }

    return 0;
}
