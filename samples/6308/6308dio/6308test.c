#include <stdio.h>
#include <unistd.h>
#include "dask.h"
#include "conio.h"

int main( void )
{
    I16 card = -1, card_number = 0;
    U32 input, out_value = 0;

    if((card = Register_Card(PCI_6308V, card_number))<0){
        printf("Can't open device file: PCI6308V\n");
        exit(-1);
    }

    do{
        clrscr();
        printf("PCI-6308V DIO Sample :\n\n");
        printf("-------------------------------------------------\n");
        DO_WritePort(card, 0, out_value);
        printf(" DO port output = 0x%x\n", (U16)(out_value&0x0f));
        usleep(100);
        DI_ReadPort(card,  0, &input);
        printf(" DI port input = 0x%x\n", (U16)(input&0x0f));
        printf("-------------------------------------------------\n");
        //if(stop_flag) break;
        printf(" Press ENTER to stop...\n");
        //usleep(100000);
        out_value = (out_value+1)%16;
    }while(!kbhit());

    if(card>=0){
        Release_Card(card);
    }
    return 0;
}
