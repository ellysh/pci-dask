#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "dask.h"
#include "conio.h"

#define Max_Chan 4


int main( void )
{
    int i;
    I16 card, err;
    I16 range=AD_B_5_V;
    char temp_char;
    I16 chan_data[Max_Chan];
    F32 chan_voltage[Max_Chan], max_voltage=5;

    setbuf( stdout, NULL );
    if ((card=Register_Card (PCI_9116, 0)) <0 ) {
        printf("Register_Card error=%d\n", card);
        exit(1);
    }

    clrscr();
    printf("*****************************************************************" );
    printf("                       cPCI9116 AI Polling                       \n");
    printf("*****************************************************************\n\n");
    printf(" please select the AI input range :                              \n");
    printf("(1)AD_B_5_V     (2)AD_B_2_5_V   (3)AD_B_1_25_V:");
    temp_char = getch();
    if(  temp_char < 0x31 || temp_char > 0x33 ){
        range = AD_B_5_V; max_voltage = 5;
        printf(" AI input range incorrect , use the default value 'AD_B_5_V'.\n");
        printf("                                    pesss any key to continue\n");
        getch();
    }
    else {
        range = temp_char - 0x30 + 1;
        max_voltage = (F32)5 / (1<<(range-2));
    }

    do{
        system( "clear" );
        printf("*****************************************************************\n" );
        printf("                      cPCI9116 AI Polling                        \n");
        printf("*****************************************************************\n\n");
        printf("                                                                 \n\n");
        for( i=0 ; i<Max_Chan; i++ ){
            if( (err = AI_ReadChannel(card, i, range, (U16*)&chan_data[i]) ) != NoError )
                printf(" AI_ReadChannel Ch#%d error : error_code: %d \n", i, err );
            chan_data[i] = chan_data[i];
            chan_voltage[i] = (((F32) chan_data[i]) / 32768)* max_voltage ;
            chan_data[i] = chan_data[i] & 0xffff;
        }

        printf("=================  Analog Input =================\n");
        printf("                Ch0       Ch1       Ch2       Ch3\n");
        printf(" input value :  %04x       %04x       %04x       %04x\n", \
            chan_data[0], chan_data[1], chan_data[2], chan_data[3]);
        printf("     voltage :  %.2f      %.2f      %.2f      %.2f\n\n", \
            chan_voltage[0], chan_voltage[1], chan_voltage[2], chan_voltage[3]);

        printf("\n\n\n                                       press Enter to stop \n");
        usleep( 50000 );

    }while(!kbhit() );

    Release_Card( card );

    return 0;
}
