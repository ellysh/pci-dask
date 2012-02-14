#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "dask.h"
#include "conio.h"

I16 card=-1, err;

void wdt_sig_handler(int signo)
{
    U32 result;
    printf("\nWDT is overflowed!!\n");
    usleep(1);

    DO_ReadPort (card, 0, &result);
    printf("\nThe CH0 current pattern is : %x\n", result);
    DO_ReadPort (card, 1, &result);
    printf("The CH1 current pattern is : %x\n", result);
    DO_ReadPort (card, 2, &result);
    printf("The CH2 current pattern is : %x\n", result);
    DO_ReadPort (card, 3, &result);
    printf("The CH3 current pattern is : %x\n", result);

    WDT_Control (card, 0, WDT_DISARM);
    //DIO_SetDualInterrupt(card, -1, -1, NULL, NULL);
    printf("\nPress any key to exit...\n" );

    return;
}

int main(void)
{
    I16 card_num;
    U32 pattern = 0, pattern0 = 0, pattern1 = 0, pattern2 = 0, pattern3 = 0;
    F32 rt = 0.0;

    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if ((card=Register_Card (PCI_7444, card_num)) <0 ) {
        printf("Register_Card error=%d\n", card);
        exit(1);
    }

    printf("Please input the WDT SafetyOut Pattern of CH0: ");
    scanf(" %x",&pattern0);
    printf("Please input the WDT SafetyOut Pattern of CH1: ");
    scanf(" %x",&pattern1);
    printf("Please input the WDT SafetyOut Pattern of CH2: ");
    scanf(" %x",&pattern2);
    printf("Please input the WDT SafetyOut Pattern of CH3: ");
    scanf(" %x",&pattern3);

    err = SetInitPattern (card, SAFTOUT_PTN_CH0, pattern0);
    if(err != NoError) {
        printf("setpattern err %d!", err);
        Release_Card(card);
        exit(1);
    }
    err = SetInitPattern (card, SAFTOUT_PTN_CH1, pattern1);
    if(err != NoError) {
        printf("setpattern err %d!", err);
        Release_Card(card);
        exit(1);
    }
    err = SetInitPattern (card, SAFTOUT_PTN_CH2, pattern2);
    if(err != NoError) {
        printf("setpattern err %d!", err);
        Release_Card(card);
        exit(1);
    }
    err = SetInitPattern (card, SAFTOUT_PTN_CH3, pattern3);
    if(err != NoError) {
        printf("setpattern err %d!", err);
        Release_Card(card);
        exit(1);
    }

    usleep(500000);

    GetInitPattern (card, SAFTOUT_PTN_CH0, &pattern);
    if(err != NoError) {
        printf("getpattern err %d!", err);
        Release_Card(card);
        exit(1);
    }
    printf("\nWDT SafetyOut Pattern of CH0: %x\n", pattern);
    GetInitPattern (card, SAFTOUT_PTN_CH1, &pattern);
    if(err != NoError) {
        printf("getpattern err %d!", err);
        Release_Card(card);
        exit(1);
    }
    printf("WDT SafetyOut Pattern of CH1: %x\n", pattern);
    GetInitPattern (card, SAFTOUT_PTN_CH2, &pattern);
    if(err != NoError) {
        printf("getpattern err %d!", err);
        Release_Card(card);
        exit(1);
    }
    printf("WDT SafetyOut Pattern of CH2: %x\n", pattern);
    GetInitPattern (card, SAFTOUT_PTN_CH3, &pattern);
    if(err != NoError) {
        printf("getpattern err %d!", err);
        Release_Card(card);
        exit(1);
    }
    printf("WDT SafetyOut Pattern of CH3: %x\n", pattern);

    err = WDT_Setup (card, 0 | WDT_OVRFLOW_SAFETYOUT, 5.0, &rt, wdt_sig_handler/*NULL*/);
    if(err != NoError) {
        printf("WDT_Setup err %d!", err);
        Release_Card(card);
        exit(1);
    }

    err = WDT_Control (card, 0, WDT_ARM);
    if(err != NoError) {
        printf("WDT_Control err %d!", err);
        Release_Card(card);
        exit(1);
    }
    /*
     * You can also replace the above function, WDT_Control, by the following function,
     * DIO_SetDualInterrupt, to enable the WDT. Remeber to let the last argument of
     * WDT_Setup function to be NULL since the signal handler is assigned in
     * DIO_SetDualInterrupt function.
     */
    /*
    err = DIO_SetDualInterrupt(card, -1, INT2_WDT,  NULL, wdt_sig_handler);
    if(err != NoError){
        printf("error : %d\n", err);
        Release_Card(card);
        exit(1);
    }
    */

    /*you can use the following function for reloading WDT counter before WDT overflow*/
    err = WDT_Reload (card, 0, 2.0, &rt);
    if(err != NoError) {
        printf("WDT_Reload err %d!", err);
        Release_Card(card);
        exit(1);
    }

    printf("\nWDT will overflow after %.4f seconds ...\n", rt);
    getch();

    Release_Card(card);
}
