#include <stdio.h>
#include <fcntl.h> // for open(...)
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include "dask.h"
#include "conio.h"

int main( void )
{
    int span, input_sec;
    char temp_char;
    I16 card, err;
    I16 range=AD_B_10_V;
    I16 DA_value;
    F32 out_voltage;
    BOOLEAN bstop = FALSE;

    setbuf( stdout, NULL );
    if ((card=Register_Card (PCI_9111DG, 0)) <0 ) {
        printf("Register_Card error=%d\n", card);
        exit(1);
    }

    clrscr();
    printf("*****************************************************************" );
    printf("                                          PCI9111 AO\n");
    printf("*****************************************************************\n\n");
    printf(" select analog output range : ( please check JP1 jumper setting )\n");
    printf(" (1)Bipolar [-10v ~ +10v]    (2)Unipolar [0v ~ 10v]  :");
    temp_char = getch();
    switch( temp_char )
    {
        case 0x32:
            range = AD_U_10_V; span = 10;
            break;
        default:
            printf(" AO output range uncorrect ,use the default value [ -10v~+10v ].\n");
            getchar();
            input_sec = 0;
            range = AD_B_10_V; span = 20;
            goto voltage_input_section;
        case 0x31:
            range = AD_B_10_V; span = 20;
            break;
    }

    input_sec = temp_char - 0x31;
    printf("\n Please input output Voltage value in [%s]:", input_sec ? "0v ~ 10v" : "-10v ~ +10v");
    scanf("%f", &out_voltage ); getchar();

    do{
        if( range == AD_U_10_V ){
            if( out_voltage < 0 ){ // Unipolar
                printf("\n !! Minus value cannot be used in Unipolar mode !!\n\n");
                goto voltage_input_section;
            }
            else if( out_voltage > 10 ){
                printf("\n !! the voltage is out of valid range [0v ~ 10v] !!\n\n");
                goto voltage_input_section;
            }
            DA_value = (I16)( (out_voltage)*4095 / span );
        }
        else if( range == AD_B_10_V ){
            if( out_voltage > 10 || out_voltage < -10 ){
                printf("\n !! the voltage is out of valid range[-10v ~ +10v] !!\n\n");
                goto voltage_input_section;
            }
            DA_value = (I16)( (out_voltage+10)*4095 / span );
        }

        if( (err = AO_WriteChannel(card, 0, DA_value)) != NoError ){
            printf(" AO_WriteChannel error : error_code: %d \n", err );
            Release_Card( card );
            exit( -1 );
        }

        clrscr();
        printf("*****************************************************************\n" );
        printf("                                          PCI9111 AO\n");
        printf("*****************************************************************\n\n");
        printf(" Output Voltage : %f V (0x%x) ", out_voltage, DA_value);
        printf("\n");
        usleep( 100000 );
voltage_input_section:
        printf("\n\n                            press (C)ontinue or any key to Quit :");
        temp_char = getch();

        if( temp_char != 'C' && temp_char != 'c' ){
            bstop = TRUE;
            AO_WriteChannel(card, 0, 0);
            goto end_process;
        }
        else
            bstop = FALSE;
        printf("\n\n Please input output Voltage value in [%s]:", input_sec ? "0v ~ 10v" : "-10v ~ +10v");
        scanf("%f", &out_voltage ); getchar();
    }while( !bstop );

end_process:
    Release_Card( card );
  return 0;
}
