#include <stdio.h>
#include "dask.h"
#include "conio.h"


int main( void )
{
    I16 card=-1, card_number = 0;
    U32 input = 0;

    if ((card=Register_Card(PCI_7433, card_number)) < 0) {
        printf ("Can't open device file: PCI7433\n");
        exit(-1);
    }

    printf ("open device file successfully!!\n");

    do {
        clrscr();
        printf ("PCI-7433R DIO Sample :\n\n");
        printf ("The Data from DI are shown in the following table:\n");
        printf ("--------------------------------------------------\n");
        DI_ReadPort(card,  PORT_DI_LOW, &input);
        printf(" DI low port input = 0x%lx\n", input);
        DI_ReadPort(card,  PORT_DI_HIGH, &input);
        printf(" DI high port input = 0x%lx\n", input);
        DO_WritePort(card, P7433R_DO_LED, input);
        printf(" LED output = 0x%lx\n", (input));
        printf ("--------------------------------------------------\n");
        printf (" Press Enter to stop...\n");
        //usleep(100000);
    }while( !kbhit() );

    if(card>=0) {
        Release_Card(card);
    }
    return 0;
}
