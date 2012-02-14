#include <stdio.h>
#include <unistd.h>
#include "dask.h"
#include "conio.h"

int main(void)
{
    I16 card = -1, card_number = 0;
    U32 input = 0, out_value = 0;

    if((card = Register_Card(PCI_7252, card_number))<0){
        printf("Can't open device file: PCI7252\n");
        exit(-1);
    }
    printf("open device file successfully!!\n");

    do{
        clrscr();
        printf("PCI-7252 DIO Sample :\n");
        printf("\nThe Data to/from DO/DI are shown in the following table:\n");
        printf("--------------------------------------------------------\n");
        DO_WritePort(card, 0, out_value);
        printf(" Do port output = 0x%x\n", (U8)out_value);
        usleep(100);
        DI_ReadPort(card,  0, &input);
        printf(" DI port input = 0x%x\n", (U16)input);
        printf("--------------------------------------------------------\n");
        printf(" Press Enter to stop...\n");
        //usleep(200000);
        out_value = (out_value+1)%256;
    }while(!kbhit());

    if(card>=0){
        Release_Card(card);
    }
    return 0;
}
