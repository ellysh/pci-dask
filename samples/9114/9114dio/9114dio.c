#include <stdio.h>
#include <stdio.h>
#include <fcntl.h> // for open(...)
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include "dask.h"
#include "conio.h"

int main( void )
{
    I16 card;
    U32 input = 0, output_value = 0;

    if ((card=Register_Card (PCI_9114DG, 0)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }


    printf("\n Press Enter for Exit \n");

    do{
        clrscr();
        printf("*****************************************************************\n" );
        printf("                      PCI9114 IO Polling                         \n");
        printf("*****************************************************************\n\n");
        printf("                                                                 \n\n\n\n");
        DO_WritePort(card,0, output_value);
        printf("        Value write to OUTPUT PORT : %04x                        \n\n", (U16)output_value);
        DI_ReadPort(card,0,&input);
        printf("        Value read from INPUT PORT : %04x                        \n\n\n\n\n\n\n", (U16)input );
        printf("                                        press Enter to stop      \n\n\n");

        usleep( 10000 );
        if( output_value == 0xffff )
            output_value = 0;
        else
            output_value ++;
    }while( !kbhit() );

    Release_Card( card );
    return 0;
}
