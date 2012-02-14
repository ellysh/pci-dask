#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "dask.h"
#include "conio.h"

I16 card = -1;

void wdt_sig_handler(int signo)
{
    U8 sts = 0;
    U32 input = 0;

    printf("\nWDT is overflowed!!\n");
    EMGShutDownStatus(card, &sts);
    if(sts)
        EMGShutDownControl(card, EMGSHDN_RECOVERY);
    usleep(10000);
    DO_ReadPort(card, 0, &input);
    printf("\nThe current pattern is : %x\n", input);
    printf("\nPress any key to exit ...\n");

    return;
}

int main(void)
{
    I16 err;
    int card_number = 0;
    U32 input = 0, out_value = 1, pattern = 0;
    F32 rt = 0.0;

    printf(" Please input the Card_Index for PCI7260 :");
    scanf(" %d", &card_number ); getchar();

    if((card = Register_Card(PCI_7260, (I16)card_number))<0){
        printf ("Can't open device file: PCI7260 %d\n", card);
        exit(-1);
    }
    printf("open device file successfully!!\n");
    err = IdentifyLED_Control (card, 1);
    GetInitPattern(card, INIT_PTN, &pattern);
    printf("Init Pattern: %x\n", pattern);
    DO_WritePort(card, 0, pattern);
    GetInitPattern(card, EMGSHDN_PTN, &pattern);
    printf("EMG Shutdown Pattern: %x\n", pattern);

    err = WDT_Setup(card, 0, 5.0, &rt, wdt_sig_handler);
    if(err!=NoError){
        printf("WDT_Setup err %d!", err);
        Release_Card(card);
        exit(1);
    }
    err = WDT_Control(card, 0, WDT_ARM);
    if(err!=NoError){
        printf("WDT_Control err %d!", err);
        Release_Card(card);
        exit(1);
    }
    printf("\nWDT will overflow after %.4f seconds ...\n", rt);
    getch();
    WDT_Control(card, 0, WDT_DISARM);
    IdentifyLED_Control(card, 0);

    if(card>=0){
        Release_Card(card);
    }
    return 0;
}
