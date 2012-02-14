#include <stdio.h>
#include <stdlib.h>
#include "conio.h"
#include "dask.h"

void main(void)
{
    I16 card, err, card_num=0;
    U32 out_value = 0;
    U32 read_out_value;
    U32 in_value;

    printf("This program outputs data to DO Port 0/1, and inputs data from DI Port 0/1\n");
    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if ((card=Register_Card (PCI_7442, card_num)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

   do{
       clrscr();
       printf ("-------------------------------------------------\n");

        err = DO_WritePort(card, P7442_CH0, out_value);
        if (err!=NoError) {
            printf("DO_WritePort error=%d", err);
            exit(1);
        }
        printf(" P0 output = 0x%x\n", out_value);

        err = DO_WritePort(card, P7442_CH1, out_value);
        if (err!=NoError) {
            printf("DO_WritePort error=%d", err);
            exit(1);
        }
        printf(" P1 output = 0x%x\n", out_value);

        printf ("-------------------------------------------------\n");
        usleep(1000);

        err = DI_ReadPort(card,  P7442_CH0, &in_value);
        if (err!=NoError) {
            printf("DI_ReadPort error=%d", err);
            exit(1);
        }
        printf(" P0 input = 0x%x\n", in_value);

        err = DI_ReadPort(card,  P7442_CH1, &in_value);
        if (err!=NoError) {
            printf("DI_ReadPort error=%d", err);
            exit(1);
        }
        printf(" P1 input = 0x%x\n", in_value);

        printf ("-------------------------------------------------\n");
        printf ("\n Press Enter to stop...\n");

        if(out_value < 0xffffffff)
            out_value = (out_value + 1);
        else
            out_value = 0;
    } while(!kbhit());

    Release_Card(card);
}
