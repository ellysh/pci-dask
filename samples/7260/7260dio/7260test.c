#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "dask.h"
#include "conio.h"


int main(void)
{
    I16 card = -1, err;
    int card_number = 0;
    U32 input = 0, out_value = 1, pattern=0;
    printf(" Please input the Card_Index for PCI7260 :");
    scanf(" %d", &card_number); getchar();

    if((card = Register_Card(PCI_7260, (I16)card_number))<0){
        printf("Can't open device file: PCI7260 %d\n", card);
        exit(-1);
    }
    printf("open device file successfully!!\n");
    err = IdentifyLED_Control(card, 1);
    GetInitPattern(card, INIT_PTN, &pattern);
    printf("Init Pattern: %x\n", pattern);
    GetInitPattern(card, EMGSHDN_PTN, &pattern);
    printf("EMG Shutdown Pattern: %x\n", pattern);
    do{
        clrscr();
        printf("PCI-7260 DIO Sample :\n");
        printf("\nThe Data to/from DO/DI are shown in the following table:\n");
        printf("--------------------------------------------------------\n");
        err = DO_WritePort(card, 0, out_value);
        printf(" Do port output = 0x%x err=%d\n", (U8)out_value, err);
        usleep(10000);
        err = DI_ReadPort(card,  0, &input);
        printf(" DI port input = 0x%x  err=%d\n", (U8)input, err);
        printf("--------------------------------------------------------\n");
        printf(" Press Enter to stop...\n");
        usleep(200000);
        out_value = /*(out_value + 1)*/(out_value*2)%256;
        if(!out_value)
            out_value = 1;
    }while(!kbhit());

    if(card>=0){
        Release_Card(card);
    }
    return 0;
}
