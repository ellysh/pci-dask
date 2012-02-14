#include <stdio.h>
#include <stdlib.h>
#include "conio.h"
#include "dask.h"

void main(void)
{
    I16 card, err, card_num=0;
    U32 out_value = 0;
    U32 read_out_value;

    printf("This program outputs data to DO Port 0/1/2/3, and reads data back from DO Port 0/1/2/3\n");
    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if ((card=Register_Card (PCI_7444, card_num)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

    do{
        clrscr();
        printf ("-------------------------------------------------\n");

        err = DO_WritePort(card, P7444_CH0, out_value);
        if (err!=NoError) {
            printf("DO_WritePort error=%d", err);
            exit(1);
        }
        printf(" P0 output = 0x%x\n", out_value);

        err = DO_WritePort(card, P7444_CH1, out_value);
        if (err!=NoError) {
            printf("DO_WritePort error=%d", err);
            exit(1);
        }
        printf(" P1 output = 0x%x\n", out_value);

        err = DO_WritePort(card, P7444_CH2, out_value);
        if (err!=NoError) {
            printf("DO_WritePort error=%d", err);
            exit(1);
        }
        printf(" P2 output = 0x%x\n", out_value);

        err = DO_WritePort(card, P7444_CH3, out_value);
        if (err!=NoError) {
            printf("DO_WritePort error=%d", err);
            exit(1);
        }
        printf(" P3 output = 0x%x\n", out_value);

        printf ("-------------------------------------------------\n");
        usleep(100);

        err = DO_ReadPort(card, P7444_CH0, &read_out_value);
        if (err!=NoError) {
            printf("DO_ReadPort error=%d", err);
            exit(1);
        }
        printf(" P0 readback = 0x%x\n", read_out_value);

        err = DO_ReadPort(card,  P7444_CH1, &read_out_value);
        if (err!=NoError) {
            printf("DO_ReadPort error=%d", err);
            exit(1);
        }
        printf(" P1 readback = 0x%x\n", read_out_value);

        err = DO_ReadPort(card, P7444_CH2, &read_out_value);
        if (err!=NoError) {
            printf("DO_ReadPort error=%d", err);
            exit(1);
        }
        printf(" P2 readback = 0x%x\n", read_out_value);

        err = DO_ReadPort(card,  P7444_CH3, &read_out_value);
        if (err!=NoError) {
            printf("DO_ReadPort error=%d", err);
            exit(1);
        }
        printf(" P3 readback = 0x%x\n", read_out_value);

        printf ("-------------------------------------------------\n");

        printf ("\n Press Enter to stop...\n");
        usleep(100000);
        if(out_value < 0xffffffff)
            out_value = (out_value + 1);
        else
            out_value = 0;
    } while(!kbhit());

    Release_Card(card);
}
