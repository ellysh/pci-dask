#include <stdio.h>
#include <unistd.h>
#include "dask.h"
#include "conio.h"

I16 card=-1, card_number=0;
U32 event_count=0;
U32 new_value, old_value=65535;

void sig1_handler( int signo )
{
    CTR_Read (card, 1, &new_value);
    event_count = old_value-new_value;
    old_value = new_value;
}

int main(void)
{
    int err;
    char temp_char;

    setbuf( stdout, NULL );

    printf("PCI-8554 Event Count (INT1)\n");
    printf("Connect measured signal to the clock source of the counter #1.\n");
    printf("It will measure event count every second.\n");

    if ((card=Register_Card(PCI_8554, card_number)) < 0) {
        printf("Register_Card Error = %d\n", card);
        exit(0);
    }

    err = DIO_SetDualInterrupt (card, INT1_COUT12, INT2_DISABLE, sig1_handler, NULL);
    if( err != NoError ){
        printf(" error from DIO_SetDualInterrupt : %d \n" , err );
        goto exit_after_error;
    }

    CTR_8554_ClkSrc_Config (card, 1, ECKN);
    CTR_Setup (card, 11, 3, 4000, BIN);
    CTR_Setup (card, 12, 2, 2000, BIN);
    CTR_Setup (card, 1, 2, 65535, BIN);
    printf("press any key to show the Event Count.Or press ESC for exit\n\n" );

    while(1){
        temp_char = getch();
        if( temp_char == 0x1b )
            break;

        printf("Event count value is %ld\n", event_count);
        printf("press any key to show the Event Count.Or press ESC for exit\n\n" );
    }

    DIO_SetDualInterrupt (card, -1, -1, NULL, NULL);

exit_after_error:
    if (card >= 0)
        Release_Card(card);
    exit(0);
}
