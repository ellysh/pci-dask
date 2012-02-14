/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright (c) 1995-1999, ADLink Technology Inc.  All rights reserved.  ;;
;;                                                                         ;;
;;      File Name : 7396INT.C                                              ;;
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
    printf("INT count = %d\n\n", int_count);
    printf("press any key to generate an interrupt or press ESC for exit\n" );

    return;
}

int main(void)
{
    int  err;
    char temp_char;

    setbuf( stdout, NULL );

    printf("PCI-7396 Interrupt Sample (INT1)\n" );
    printf("This program shows the number of occurrence\n");
    printf("of COS INT1 interupt. COS INT1 interrupt is\n");
    printf("generated by external signal from DI Port 1.\n");
    printf("If you connect CN1 to CN2, press any key can generate an interrupt.\n");
    printf("Or press ESC for exit\n\n" );

    int_count=0;

    if ((card=Register_Card(PCI_7396, card_number)) < 0) {
        printf("Register_Card Error = %d\n", card);
        exit(0);
    }

    if( DIO_PortConfig(card,Channel_P1, INPUT_PORT) != NoError )
        goto exit_after_error;
    if( DIO_PortConfig(card,Channel_P2, OUTPUT_PORT) != NoError )
        goto exit_after_error;

    //DIO_SetCOSInterrupt(card, Channel_P1, 1, 1, 1);
    err = DIO_SetDualInterrupt(card, INT1_FP1C0, INT2_DISABLE, sig1_handler, NULL);
    if( err != NoError ){
        printf(" error from DIO_SetDualInterrupt : %d \n" , err );
        goto exit_after_error;
    }

    while(1){
        temp_char = getch();
        if( temp_char == 0x1b )
            break;

        DO_WritePort(card,Channel_P2C, 0x1);
        usleep(1000);
        DO_WritePort(card,Channel_P2C, 0x0);
        usleep(1000);
    }

    DIO_SetDualInterrupt(card, INT1_DISABLE, INT2_DISABLE, NULL, NULL);

exit_after_error:
    if (card >= 0)
        Release_Card(card);
    exit(0);
}
