/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright (c) 1995-1999, ADLink Technology Inc.  All rights reserved.  ;;
;;                                                                         ;;
;;      File Name : 74333int.c                                             ;;
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
    printf("press ENTER for exit\n\n" );

    return;
}

int main(void)
{
    int  err;

    printf("PCI-7433 Interrupt Sample (INT1)\n\n" );
    printf("This program shows the number of occurrence\n");
    printf("of INT1 interupt. INT1 interrupt is generated\n");
    printf("by external signal from DI bit-0.\n\n");
    printf("Press ENTER for exit\n\n" );

    int_count = 0;

    if ((card=Register_Card(PCI_7433, card_number)) < 0) {
        printf("Register_Card Error = %d\n", card);
        exit(0);
    }

    err = DIO_SetDualInterrupt(card, INT1_EXT_SIGNAL, INT2_DISABLE, sig1_handler, NULL);
    if( err != NoError ){
        printf(" error from DIO_SetDualInterrupt : %d \n" , err );
        goto exit_after_error;
    }

    getchar();

    DIO_SetDualInterrupt(card, INT1_DISABLE, INT2_DISABLE, NULL, NULL);

exit_after_error:
    if (card >= 0)
        Release_Card(card);
    exit(0);
}
