#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "dask.h"
#include "conio.h"

I16 card=-1, err;

void wdt_sig_handler( int signo )
{
    printf("\nWDT is overflowed!!\n");
    WDT_Control (card, 0, WDT_DISARM);
    printf("\nPress any key to exit...\n" );

    return;
}

int main( void )
{
    int card_number = 0;
    U32 input = 0, out_value = 1, pattern=0;
    F32 rt = 0.0;
    U8 sts = 0;

    printf("Please input the Card_Index for PCI7452 :");
    scanf("%d", &card_number ); getchar();

    if ((card=Register_Card(PCI_7452, (I16)card_number)) < 0) {
        printf ("Can't open device file: PCI7452 %d\n", card);
        exit(-1);
    }

    err = WDT_Setup (card, 0, 5.0, &rt, wdt_sig_handler);
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
    printf("\n\nWDT will overflow after %.4f seconds ...\n", rt);
    getch();
    WDT_Control (card, 0, WDT_DISARM);

    if(card>=0) {
        Release_Card(card);
    }
    return 0;
}
