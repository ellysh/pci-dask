/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright (c) 1995-1999, ADLink Technology Inc.  All rights reserved.  ;;
;;                                                                         ;;
;;      File Name : 7432INT.C                                              ;;
;;      Purpose   : This sample program demonstrates how handle interrupt  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;*/
#include <stdio.h>
#include <unistd.h>
#include "dask.h"
#include "conio.h"

int int_count=0;
I16 card=-1, card_number = 0;

void sig1_handler( int signo )
{
    int_count++;
    printf("INT count = %d\n", int_count);
    printf("press any key to generate an interrupt or press ESC for exit\n\n" );

    return;
}

int main(void)
{
    int  err;
    char temp_char;

    printf("PCI-7432 Interrupt Sample (INT1)\n" );
    printf("This program shows the number of occurrence\n");
    printf("of INT1 interupt. INT1 interrupt is generated\n");
    printf("by external signal from DI bit-0. If you connect\n");
    printf("DO bit-0 to DI bit-0, press any key can\n");
    printf("generate an interrupt.\n\n\n");

    int_count = 0;

    if ((card=Register_Card(PCI_7432, card_number)) < 0) {
        printf("Register_Card Error = %d\n", card);
        exit(0);
    }

    err = DIO_SetDualInterrupt(card, INT1_EXT_SIGNAL, INT2_DISABLE, sig1_handler, NULL);
    if( err != NoError ){
        printf(" error from DIO_SetDualInterrupt : %d \n" , err );
        goto exit_after_error;
    }

    while(1){
        temp_char = getch();
        if( temp_char == 0x1b )
            break;

        DO_WritePort(card,0, 0x1);
        usleep(1000);
        DO_WritePort(card,0, 0x0);

    }

    DIO_SetDualInterrupt(card, INT1_DISABLE, INT2_DISABLE, NULL, NULL);

exit_after_error:
    if (card >= 0)
        Release_Card(card);
    exit(0);
}
