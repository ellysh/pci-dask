#include <stdio.h>
#include <unistd.h>
#include "dask.h"
#include "conio.h"

int main(void)
{
    I16 card = -1, card_number = 0;
    U32 input, out_value = 0;

    if((card = Register_Card(PCI_6202, card_number))<0){
        printf("Register_Card %d error: %d", card_number, card);
        exit(-1);
    }

    do{
        clrscr();
        printf("PCI-6202 DIO Sample :\n");
        printf("\nThe Data to/from DO/DI are shown in the following table:\n");
        printf("--------------------------------------------------------\n");
        DO_WritePort(card, 1, out_value);
        printf(" DO port output (TTL) = 0x%x\n", (U16)(out_value & 0x0ff));
        usleep(1000);
        DI_ReadPort(card, 1, &input);
        printf(" DI port input (TTL) = 0x%x\n", (U16)(input & 0x0ff));
        printf("--------------------------------------------------------\n");
        //if(stop_flag) break;
        printf(" Press Enter to stop...\n");
        //usleep(100000);
        out_value = (out_value+1)%256;
    }while(!kbhit());

    if(card>=0){
        Release_Card(card);
    }
    return 0;
}
