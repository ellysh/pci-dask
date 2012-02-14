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

    printf("This sample outputs voltage value to AO channel\n\n");
    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if((card=Register_Card(PCI_9221, card_num))<0){
        printf("Register_Card error=%d", card);
        exit(1);
    }

    do{
        clrscr();

        printf("Input voltage value to be written to AO CH0 (-5 ~ +5): ");
        scanf(" %f", &out_data);
        if((out_data>5)||(out_data<-5)){
            printf("\nInvalid Value, Output Range is between -5 and +5\n");
            goto cont_loop;
        }
        err = AO_VWriteChannel(card, 0, (F64)out_data);
        if(err < 0){
            printf("AO Write Ch0 Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }

        printf("Input voltage value to be written to AO CH1 (-5 ~ +5): ");
        scanf(" %f", &out_data);
        if((out_data>5)||(out_data<-5)){
            printf("\nInvalid Value, Output Range is between -5 and +5\n");
            goto cont_loop;
        }
        err = AO_VWriteChannel(card, 1, (F64)out_data);
        if(err<0){
            printf("AO Write Ch1 Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }

cont_loop:
        printf("\nPress any key to continue, and 'q' to exit...\n");
        qflag = getch();
    }while(qflag!='q');

    Release_Card(card);

    return 0;
}
