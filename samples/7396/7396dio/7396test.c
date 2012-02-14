#include <stdio.h>
#include "dask.h"
#include "conio.h"


int main( void )
{
    I16 card=-1, card_number = 0;
    U32 input, out_value = 0;

    if ((card=Register_Card(PCI_7396, card_number)) < 0) {
        printf ("Can't open device file: PCI7396\n");
        exit(-1);
    }
    printf ("open device file successfully!!\n");

    DIO_PortConfig(card,Channel_P1A, INPUT_PORT);
    DIO_PortConfig(card,Channel_P2A, OUTPUT_PORT);
    DIO_PortConfig(card,Channel_P1B, INPUT_PORT);
    DIO_PortConfig(card,Channel_P2B, OUTPUT_PORT);
    DIO_PortConfig(card,Channel_P1C, INPUT_PORT);
    DIO_PortConfig(card,Channel_P2C, OUTPUT_PORT);
    clrscr();
    printf ("PCI-7396 DIO Sample :\n\n");
    printf ("-------------------------------------------------\n");
    printf (" P1A, p1B, P1C are configured as input port.\n");
    printf (" P2A, p2B, P2C are configured as output port.\n");
    printf ("-------------------------------------------------\n");
    printf (" Press any key to continue...\n");
    getch();

    do {
        clrscr();
        printf ("PCI-7396 DIO Sample :\n\n");
        printf ("-------------------------------------------------\n");
        printf (" P1A, p1B, P1C are configured as input port.\n");
        printf (" P2A, p2B, P2C are configured as output port.\n\n");
        DO_WritePort(card, Channel_P2A, out_value);
        printf(" P2A output = 0x%x\n", (U16)out_value);
        DI_ReadPort(card,  Channel_P1A, &input);
        printf(" P1A input = 0x%x\n", (U16)input);
        DO_WritePort(card, Channel_P2B, out_value);
        printf(" P2B output = 0x%x\n", (U16)out_value);
        DI_ReadPort(card,  Channel_P1B, &input);
        printf(" P1B input = 0x%x\n", (U16)input);
        DO_WritePort(card, Channel_P2C, out_value);
        printf(" P2C output = 0x%x\n", (U16)out_value);
        DI_ReadPort(card,  Channel_P1C, &input);
        printf(" P1C input = 0x%x\n", (U16)input);
        printf ("-------------------------------------------------\n");
        printf (" Press Enter to stop...\n");
        //usleep(200000);
        out_value = (out_value + 1) % 256;
    }while( !kbhit() );

    if(card>=0) {
        Release_Card(card);
    }
    return 0;
}
