#include <stdio.h>
#include <unistd.h>
#include "dask.h"
#include "conio.h"


int main( void )
{
    I16 card=-1, card_number = 0;
    U32 input = 0, out_value = 1;

    if ((card=Register_Card(PCI_7432, card_number)) < 0) {
        printf ("Can't open device file: PCI7432\n");
        exit(-1);
    }
    printf ("open device file successfully!!\n");

    do {
        clrscr();
        printf ("PCI-7432 DIO Sample :\n\n");
        printf ("The Data to/from DO/DI are shown in the following table:\n");
        printf ("--------------------------------------------------------\n");
        DO_WritePort(card, 0, out_value);
        printf(" Do port output = 0x%lx\n", out_value);
        usleep(100);
        DI_ReadPort(card,  0, &input);
        printf(" DI port input  = 0x%lx\n", input);
        printf ("--------------------------------------------------------\n");
        printf (" Press Enter to stop...\n");
        //usleep(100000);
        out_value = (out_value * 2) % ((1<<31) + 1);
        if(!out_value)
            out_value = 1;
    }while( !kbhit() );

    if(card>=0) {
        Release_Card(card);
    }
    return 0;
}
