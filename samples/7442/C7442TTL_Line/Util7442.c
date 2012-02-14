#include <stdio.h>
#include <unistd.h>
#include "conio.h"
#include "dask.h"


void main(void)
{

    I16 card, err, card_num=0;
    U16 value;
    U16 out_value = 0;

    printf("This program output data from TTL0's Line1 to TTL1's Line1,\n");
    printf("         and output data from TTL1's Line0 to TTL0's Line0.\n");
    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if ((card = Register_Card(PCI_7442, card_num)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

    err = DIO_LineConfig(card, P7442_TTL0, 0, INPUT_PORT); // set Line0 of TTL0 to input
    if(err!=NoError) {
        printf("TTL Port0 Input Config Error: %d\n",err);
        exit(1);
    }

    err = DIO_LineConfig(card, P7442_TTL0, 1, OUTPUT_PORT); // set Line1 of TTL0 to output
    if(err!=NoError) {
        printf("TTL Port0 Output Config Error: %d\n",err);
        exit(1);
    }

    err = DIO_LinesConfig(card, P7442_TTL1, 0x1); // set Line0 of TTL1 to output
    if(err!=NoError) {
        printf("TTL Port1 Config Error: %d\n",err);
        exit(1);
    }

    do{
        clrscr();
        printf ("-------------------------------------------------\n");

        err = DO_WriteLine(card, P7442_TTL0, 1, out_value);
        if (err!=NoError) {
            printf("DO_WriteLine error=%d", err);
            exit(1);
        }
        printf(" TTL0 (Line1) output = %d\n", out_value);

        out_value = out_value? 0:1;
        err = DO_WriteLine(card, P7442_TTL1, 0, out_value);
        if (err!=NoError) {
            printf("DO_WriteLine error=%d", err);
            exit(1);
        }
        printf(" TTL1 (Line0) output = %d\n", out_value);

        printf ("-------------------------------------------------\n");

        err = DI_ReadLine(card, P7442_TTL1, 1, &value);
        if (err!=NoError) {
            printf("DO_WriteLine error=%d", err);
            exit(1);
        }
        printf(" TTL1 (Line1) input = %d\n", value);

        err = DI_ReadLine(card, P7442_TTL0, 0, &value);
        if (err!=NoError) {
            printf("DO_WriteLine error=%d", err);
            exit(1);
        }
        printf(" TTL0 (Line0) input = %d\n", value);

        printf("-------------------------------------------------\n");
        printf ("\n Press Enter to stop...\n");

    }while(!kbhit());

    Release_Card(card);
}
