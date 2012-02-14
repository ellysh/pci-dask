/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright (c) 1995-1999, ADLink Technology Inc.  All rights reserved.  ;;
;;                                                                         ;;
;;      File Name   :   7300DBEVENT                                        ;;
;;      Purpose     :   This sample program demonstrates how to DIO through;;   
;;                      interrupt operation                                ;;
;;      Date        :   07/07/1999                                         ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;*/

 #include <stdio.h>
 #include <unistd.h>
 #include "dask.h"

 I16 card=-1;
 U16 int_count = 0, int2_count = 0;
 int total_count =4000;
 int sample_rate=1000;
 char Buf[80];
 int c1=20,c2=100;
 

void sig_handler1(int signo)
{
    int_count++;
    return;
}

void sig_handler2(int signo)
{
    if(int2_count < total_count) {
       int2_count++;
    }else if( int2_count == total_count ){
     // reset the Timer to stop generate INT2
       CTR_Setup (card, 2, RATE_GENERATOR, 0, BIN);
       printf("\n\n INT1 Count:%d   INT2 Count:%d \n\n\n", int_count, int2_count );
       printf(" press ENTER to exit...\n" );
       int2_count++;
   }

   return;
}

int main(void)
{
    int err;

    setbuf( stdout, NULL );
    printf("PCI-7300A Dual Interrupt Sample\n");

    int_count = int2_count = 0;

    if ((card=Register_Card (PCI_7300A_RevA, 0)) <0 ) {
      printf ("Can't open device file: PCI7300A\n");
      exit(-1);
    }
    CTR_Setup (card, 2, RATE_GENERATOR, 0, BIN);
    usleep(1000);

    err = DIO_7300SetInterrupt(card, 1, 1, sig_handler1, sig_handler2);
    if( err != NoError ){
       printf(" error from DIO_SetDualInterrupt : %d \n" , err );
       goto exit_after_error;
    }
    // setup the Timer clock
    c2=10000000/(sample_rate);
    err = CTR_Setup (card, 2, 2, c2, BIN);
    if( err != NoError ){
       printf(" error from CTR_Setup : %d \n" , err );
       goto end_of_proc;
    }
    printf("**The INT2 Singal will be sent back in 4 secs, or you can press ENTER for exit**\n");
    getchar();

    if( int2_count < total_count ){
       CTR_Setup (card, 2, RATE_GENERATOR, 0, BIN);
       goto end_of_proc;
    }

end_of_proc:
    DIO_7300SetInterrupt(card, 0, 0, NULL, NULL);
exit_after_error:
    if (card >= 0) Release_Card(card);
    exit(0);
}


