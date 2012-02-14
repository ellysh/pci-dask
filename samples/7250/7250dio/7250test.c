#include <stdio.h>
#include "dask.h"
#include "conio.h"


int main( void )
{
    I16 card = -1, card_number = 0, i;
    U32 input = 0, out_value = 1;

    if((card = Register_Card(PCI_7250, card_number))<0){
        printf("Can't open device file: PCI7250\n");
        exit(-1);
    }
    printf("open device file successfully!!\n");

    do{
        clrscr();
        printf("PCI-7250 DIO Sample :\n");
        printf("\nThe Data to/from DO/DI are shown in the following table:\n");
        printf("--------------------------------------------------------\n");
        printf(" PCI-7250   : ");
        DO_WritePort(card, 0, out_value);
        printf("Do port output = 0x%x\n", (U16)out_value);
        DI_ReadPort(card,  0, &input);
        printf("              DI port input = 0x%x\n", (U16)input);
        for(i=1; i<4; i++){
            printf(" PCI-7251#%d : ", i);
            if(DO_WritePort(card, 0, out_value)==NoError){
                printf("\nDo port output = 0x%x\n", (U16)out_value);
                DI_ReadPort(card,  0, &input);
                printf("\n            DI port input = 0x%x\n", (U16)input);
            }
            else
                printf("None\n");
        }
        printf("--------------------------------------------------------\n");
        printf(" Press Enter to stop...\n");
        //usleep(200000);
        out_value = (out_value<<1)%256;
        if(!out_value)
            out_value = 1;
    }while(!kbhit());

    if(card>=0){
        DO_WritePort(card, 0, 0);
        Release_Card(card);
    }
    return 0;
}
