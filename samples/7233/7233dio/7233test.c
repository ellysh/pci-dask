#include <stdio.h>
#include <fcntl.h>
#include "dask.h"
#include "conio.h"

int main(void)
{
    I16 card = -1, card_number = 0;
    U32 input = 0;

    if((card = Register_Card(PCI_7233, card_number))<0){
        printf("Can't open device file: PCI7233\n");
        exit(-1);
    }

    printf("open device file successfully!!\n");

    do{
        clrscr();
        printf("PCI-7233 DI Sample :\n");
        printf("\nThe Data from DI are shown in the following table:\n\n");
        printf("-------------------------------------------------\n");
        DI_ReadPort(card,  0, &input);
        printf(" DI port input = 0x%lx\n", input);
        printf("-------------------------------------------------\n");
        printf(" Press Enter to stop...\n");
        //usleep(100000);
    }while(!kbhit());

    if(card>=0){
        Release_Card(card);
    }
    return 0;
}

