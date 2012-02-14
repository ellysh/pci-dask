/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2009/06/09                                                       */
/*                                                                            */
/* This sample performs DI change of state functionality.                     */
/* In PCIe-7350, there are 32 programmable DIO channels. Each 8 channels are  */
/* devided into one port that is the DIO configuration unit.                  */
/* Internal interrupt will be asserted while the state of the input digital   */
/* signal is changed form low to high or high to low.                         */
/* AFI port can also be set as the COS event out mode, and the set AFI port   */
/* will generate a pulse while COS occurs.                                    */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "conio.h"
#include "dask.h"

I16 card=-1;
U8 int_count;
U32 COSLData, out_Val =0;

void sig_handler(int signo)
{
	switch(signo){
		case SIGEVENT4:
			int_count++;
			break;
	}

	I16 err = DIO_GetCOSLatchData32(card, 0, &COSLData);
	if(err<0){
		 printf("DIO_GetCOSLatchData32 Error: %d\n", err);
	}
	printf("Change of State; Count = %d, Latched Data: 0x%lx\n\n", int_count, COSLData);
	return;
}


int main(int argc, char **argv)
{
    I16 err;
    U16 card_num=0;
    //U32 vi;
    U32 DIRead,DOWrite;
    //BOOLEAN fStop;
    U32 COSEventOutLen = 100; //100 system clock - 100 * 8 ns

    printf("This sample performs change of state functionality.\n");
    printf("DIO Port A is configured as a DO port while Port B\n");
    printf("is configured as a DI Port. Signal will be asserted\n");
    printf("and a %ld ns pulse will be generated to AFI0 while\n", 8*COSEventOutLen);
    printf("the DI state is changed.\n");

    printf("Card Number? ");
    scanf(" %hd", &card_num);

    int_count=0;

    /*Open and Initialize Device*/
    card = Register_Card(PCI_7350, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    /*Configure 7350 DIO Port*/
    err = DIO_PortConfig(card, P7350_DIO_A, OUTPUT_PORT);
    if(err<0){
        printf("DIO_PortConfig Port_A Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    err = DIO_PortConfig(card, P7350_DIO_B, INPUT_PORT);
    if(err<0){
        printf("DIO_PortConfig Port_B Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    DO_WritePort(card, P7350_DIO_A, 0);
    //usleep(1000);

    /*
     * Setup COS for Port B
     */
    //err = DIO_SetCOSInterrupt32(card, 0, 0xff00, &COS_Event/*, 0*/);
    err = DIO_SetCOSInterrupt32(card, 0, 0xff00, sig_handler);
    if(err<0){
        printf("DIO_SetCOSInterrupt32 Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    /*
     * Set COS event out to AFI port
     */
    err = DIO_7350_AFIConfig(card, P7350_AFI_0, 1, P7350_AFI_COSTrigOut, COSEventOutLen);
    if(err<0){
        printf("DIO_7350_AFIConfig Port_A Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    do{
        /*
         * DO output to Port A/C
         */
        //printf("\nOutput Data?");
        //scanf(" %lx", &vi);
        usleep(500000);
		clrscr();
    printf("This sample performs change of state functionality.\n");
    printf("DIO Port A is configured as a DO port while Port B\n");
    printf("is configured as a DI Port. Signal will be asserted\n");
    printf("and a %ld ns pulse will be generated to AFI0 while\n", 8*COSEventOutLen);
    printf("the DI state is changed.\n\n\n");
	if((out_Val)%4==0||(out_Val)%4==1){
		DO_WritePort(card, P7350_DIO_A, 0/*vi&0xff*/);DOWrite=0;}
	else{
		DO_WritePort(card, P7350_DIO_A, 1/*vi&0xff*/);DOWrite=1;}
        //out_Val = (out_Val+1)%2;
        //DO_WritePort(card, P7350_DIO_A, out_Val/*vi&0xff*/);
	usleep(1000);
        //DO_WritePort(card, P7350_DIO_A, vi&0xff);
        DI_ReadPort(card, P7350_DIO_B, &DIRead);
	out_Val++;
        printf("Write to Port A: 0x%lx; Read from Port B: 0x%lx;\n\n\n", DOWrite/*vi&0xff*/, DIRead);
        //printf("Write to Port A: 0x%lx; Read from Port B: 0x%lx;\n", vi&0xff, DIRead);
        printf("Press Enter to Quit......\n");
        //fStop = getch();
    }while(!kbhit()/*(fStop!='q')&&(fStop!='Q')*/);

    err = DIO_SetCOSInterrupt32(card, 0, 0, NULL);
    if(err<0){
        printf("DIO_SetCOSInterrupt32 Error: %d\n", err);
    }

    err = DIO_7350_AFIConfig(card, P7350_AFI_0, 0, 0, 0);
    if(err<0){
        printf("DIO_7350_AFIConfig Error: %d\n", err);
    }

    //printf("Press any key to exit...\n\n");
    //getch();
    Release_Card(card);
    return 0;
}
