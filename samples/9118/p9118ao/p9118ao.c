#include <stdio.h>
#include <fcntl.h> // for open(...)
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include "dask.h"
#include "conio.h"

int main( void )
{
    int span=20;
    char temp_char;
    I16 card, err;
    U16 DA_value, channel;
    F32 out_voltage;
    BOOLEAN bstop = FALSE;

    setbuf( stdout, NULL );
    if ((card=Register_Card (PCI_9118DG, 0)) <0 ) {
        printf("Register_Card error=%d\n", card);
        exit(1);
    }

    clrscr();
    printf("*****************************************************************" );
    printf("                                          PCI9118 AO\n");
    printf("*****************************************************************\n\n");
    printf(" select analog output Channel : ( DAC0 / DAC1 )\n");
    printf(" (1)DAC0    (2)DAC1  :");
    temp_char = getch();
    switch( temp_char )
    {
        case 0x32:
            channel = 1;
            break;
        default:
            printf("\n AO output channel is uncorrect ,use the default channel [DAC0].\n");
        case 0x31:
            channel = 0;
            break;
    }

    printf("\n Please input output Voltage value in [-10v ~ +10v]:");
    scanf("%f", &out_voltage ); getchar();

    do{
        if( out_voltage > 10 || out_voltage < -10 ){
            printf("\n !! the voltage is out of valid range[-10v ~ +10v] !!\n\n");
            goto voltage_input_section;
        }
        DA_value = (U16)( (out_voltage +10)*4095 / span );

        if( (err=AO_WriteChannel(card, channel, DA_value)) != NoError ){
            printf(" AO_WriteChannel error : error_code: %d \n", err );
            Release_Card( card );
            exit( -1 );
        }

        clrscr();
        printf("*****************************************************************\n" );
        printf("                                          PCI9118 AO\n");
        printf("*****************************************************************\n\n");
        printf(" Output Voltage for DAC%d: %f V (0x%x) ", channel, out_voltage, DA_value);
        printf("\n");
        usleep( 100000 );
voltage_input_section:
        printf("\n\n                            press (C)ontinue or any key to Quit :");
        temp_char = getch();

        if( temp_char != 'C' && temp_char != 'c' ){
            bstop = TRUE;
            AO_WriteChannel(card, 0, 2048);
            AO_WriteChannel(card, 1, 2048);
            goto end_process;
        }
        else
            bstop = FALSE;
        printf("\n\n Please input output Voltage value in [-10v ~ +10v]:");
        scanf("%f", &out_voltage ); getchar();
    }while( !bstop );

end_process:
    Release_Card( card );

    return 0;
}
