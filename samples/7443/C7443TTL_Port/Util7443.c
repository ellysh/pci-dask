#include <stdio.h>
#include "conio.h"
#include "dask.h"
#include <unistd.h>

void main(void)
{
    I16 card, err, card_num=0;
    U16 in_value, out_value = 0;

    printf("This program output data from TTL0 to TTL1.\n");
    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if ((card = Register_Card(PCI_7443, card_num)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

    err = DIO_PortConfig(card, P7443_TTL0, OUTPUT_PORT);
    if(err!=NoError) {
        printf("Input Port Config Error: %d\n",err);
        exit(1);
    }

    err = DIO_PortConfig(card, P7443_TTL1, INPUT_PORT);
    if(err!=NoError) {
        printf("Output Port Config Error: %d\n",err);
        exit(1);
    }

    do{
        clrscr();
        printf ("-------------------------------------------------\n");

        err = DO_WritePort(card, P7443_TTL0, out_value);
        if (err!=NoError) {
            printf("DO_WritePort error=%d", err);
            exit(1);
        }
        printf(" TTL0 output = 0x%x\n", out_value);

        err = DI_ReadPort(card, P7443_TTL1, &in_value);
        if (err!=NoError) {
            printf("DI_ReadPort error=%d", err);
            exit(1);
        }
        printf(" TTL1 input = 0x%x\n", in_value);

        printf ("-------------------------------------------------\n");
        printf ("\n Press Enter to stop...\n");

        usleep(200000);
        if(out_value < 0xffff)
            out_value = (out_value + 1);
        else
            out_value = 0;
    } while(!kbhit());

    Release_Card(card);
}
