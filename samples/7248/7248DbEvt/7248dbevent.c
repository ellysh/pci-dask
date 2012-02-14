/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright (c) 1995-1999, ADLink Technology Inc.  All rights reserved.  ;;
;;                                                                         ;;
;;      File Name   :   7248INT.C                                          ;;
;;      Purpose     :   This sample program demonstrates how to DIO through;;
;;                      interrupt operation                                ;;
;;      Date        :   07/07/1999                                         ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;*/
#include <stdio.h>
#include <unistd.h>
#include "dask.h"

I16 card = -1;
U16 int_count = 0, int2_count = 0;
int c1 = 20, c2 = 100;

void sig_handler1(int signo)
{
    printf(" SIG_HANDLER1 \n"); // this will be catched by getchar() in main()
    return;
}

void sig_handler2(int signo)
{
    printf(" SIG_HANDLER2 \n"); // this will be catched by getchar() in main()
   return;
}

int main(void)
{
    int i, k = 0, err;

    setbuf(stdout, NULL);
    printf("PCI-7248 Dual Interrupt Sample\n");
    printf("This program demostrates how to use dual interrupt line.\n");
    printf("The IRQ source of INT1 is P1C0. The IRQ source of INT2 is P2C0\n");
    printf("Using cable to connect CN1 and CN2\n\n");

    if((card=Register_Card(PCI_7248, 0))<0){
        printf("Can't open device file: PCI7248\n");
        exit(-1);
    }

    //configure the port
    err = DIO_PortConfig(card,Channel_P1C, OUTPUT_PORT);
    if(err!=NoError){
        goto exit_after_error;
    }
    err = DIO_PortConfig(card,Channel_P2C, OUTPUT_PORT);
    if(err!=NoError){
        goto exit_after_error;
    }
    err = DIO_SetDualInterrupt(card, INT1_FP1C0, INT2_FP2C0, sig_handler1, sig_handler2);
    if(err!=NoError){
        printf(" eryyror from DIO_SetDualInterrupt : %d \n" , err );
        goto exit_after_error;
    }

    //generate INT1 interrupt signal
    DO_WritePort(card,Channel_P1C,1);
    DO_WritePort(card,Channel_P1C,0);
    sleep(2);

    //generate INT2 interrupt signal
    DO_WritePort(card,Channel_P2C,1);
    DO_WritePort(card,Channel_P2C,0);
    sleep(2);

end_of_proc:
    DIO_SetDualInterrupt(card, INT1_DISABLE, INT2_DISABLE, NULL, NULL);

exit_after_error:
    if(card>=0)
        Release_Card(card);
    exit(0);
}
