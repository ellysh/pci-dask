/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright (c) 1995-1999, ADLink Technology Inc.  All rights reserved.  ;;
;;                                                                         ;;
;;      File Name : 7452INT.C                                              ;;
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
   U32 CosData = 0;
   int_count++;
   DIO_GetCOSLatchDataInt32(card, 0, &CosData);
   printf("INT count: %d, COSData: %x. ", int_count, CosData);
   printf("Press any key to exit...\n" );

   return;
}

int main(void)
{
    int  err;
    char temp_char;

    printf("PCI-7452 Interrupt Sample (INT1)\n" );
    printf("This program shows the number of occurrence of COS0 interupt. \n\n");
    int_count = 0;

    if ((card=Register_Card(PCI_7452, card_number)) < 0) {
        printf("Register_Card Error = %d\n", card);
        exit(0);
    }

    //err = DIO_SetDualInterrupt(card, INT1_EXT_SIGNAL, INT2_DISABLE, sig1_handler, NULL);
    err = DIO_SetCOSInterrupt32(card, 0, 0x0001, sig1_handler);
    if( err != NoError ){
        printf(" error from DIO_SetCOSInterrupt32 : %d \n" , err );
        goto exit_after_error;
    }

    temp_char = getch();
    //if( temp_char == 0x1b ) break;

    DIO_SetCOSInterrupt32(card, 0, 0x0000, sig1_handler);

exit_after_error:
    if (card >= 0)
        Release_Card(card);
    exit(0);
}
