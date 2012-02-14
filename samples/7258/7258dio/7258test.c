#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "dask.h"
#include "conio.h"


int main(void)
{
    I16 card = -1, err;
    int card_number = 0;
    U32 input = 0, out_value = 0x80000000;

    printf(" Please input the Card_Index for PCI7258 :");
    scanf(" %d", &card_number); getchar();

    if((card = Register_Card(PCI_7258, (I16)card_number))<0){
        printf ("Can't open device file: PCI7258 %d\n", card);
        exit(-1);
    }
    printf("open device file successfully!!\n");

    do{
        clrscr();
        if(out_value==0x80000000)
            out_value = 1L;
        printf("PCI-7258 DIO Sample :\n");
        printf("\nThe Data to/from DO/DI are shown in the following table:\n");
        printf("--------------------------------------------------------\n");
        err = DO_WritePort(card, 0, out_value & 0x0FFFF);
        err = DO_WritePort(card, 1, (out_value >> 16 ) & 0x0FFFF);
        //printf(" Do port output = 0x%x\n", (U16)out_value);
        printf(" Do port output = 0x%08lx err=%d\n", (U32)out_value, err);
        usleep(5000);
        err = DI_ReadPort(card,  0, &input);
        //printf(" DI port input = 0x%x\n", (U16)input);
        printf(" DI port input = 0x%x  err=%d\n", (U16)input, err);
        printf("--------------------------------------------------------\n");
        printf(" Press Enter to stop...\n");
        //usleep(200000);
        out_value = out_value<<1;
    }while(!kbhit());

    if(card>=0){
        Release_Card(card);
    }
    return 0;
}
