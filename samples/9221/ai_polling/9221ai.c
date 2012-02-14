#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include "dask.h"

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 Channel = 0;
    U16 AdRange = AD_B_5_V; // -5V ~ +5V
    U16 acq_value;
    F64 v_acq_value;

    printf("This sample reads analog data from AI channel 0\n\n");
    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if((card=Register_Card(PCI_9221, card_num))<0){
        printf("Register_Card error=%d", card);
        exit(1);
    }

    do{
        clrscr();
        usleep(100000);
        //err = AI_ReadChannel(card, Channel, AdRange, &acq_value);
        err = AI_VReadChannel(card, Channel, AdRange, &v_acq_value);
        if(err<0){
            printf("AI Read Ch0 Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        //printf("\nAI CH0 Raw Data: 0x%x\n", acq_value);
        printf("\nAI CH0 Voltage Value: %f\n", (F32)v_acq_value);
        printf("\n\n     Press Enter to exit...\n");
    }while(!kbhit());

    Release_Card(card);
    return 0;
}
