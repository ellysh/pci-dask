#include <stdio.h>
#include "dask.h"
#include "conio.h"


int main( void )
{
    I16 card=-1, card_number = 0;
    U32 out_value = 1;

    if ((card=Register_Card(PCI_7434, card_number)) < 0) {
        printf ("Can't open device file: PCI7434\n");
        exit(-1);
    }
    printf ("open device file successfully!!\n");

    do {
        clrscr();
        printf ("PCI-7434R DO Sample :\n\n");
        printf ("The Data to DO are shown in the following table:\n");
        printf ("-------------------------------------------------\n");
        DO_WritePort(card, PORT_DO_LOW, out_value);
        printf(" Do low port output = 0x%lx\n", (out_value));
        DO_WritePort(card, PORT_DO_HIGH, out_value);
        printf(" Do high port output = 0x%lx\n", (out_value));
        DO_WritePort(card, P7434R_DO_LED, out_value);
        printf(" LED output = 0x%lx\n", (out_value));
        printf ("-------------------------------------------------\n");
        out_value = (out_value * 2) % ((1<<31) + 1);
        if(!out_value)
            out_value = 1;
        printf (" Press Enter to stop...\n");
        //usleep(100000);
    }while(!kbhit());

    if(card>=0) {
        Release_Card(card);
    }
    return 0;
}
