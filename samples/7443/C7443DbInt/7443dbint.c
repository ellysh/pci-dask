#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include "dask.h"
#include "conio.h"

int int_count=0, int2_count=0, int3_count=0, int4_count=0;
I16 card=-1, card_number;

void sig1_handler(int signo)
{
    U32 CosData = 0;
    switch(signo){
        case SIGEVENT1:
            int_count ++;
            break;
    }
    DIO_GetCOSLatchDataInt32(card, P7443_CH0, &CosData);
    printf("INT1 count = %d, COS Data: %x. \n", int_count, CosData);
    return;
}

void sig2_handler(int signo)
{
    U32 CosData = 0;
    switch(signo){
        case SIGEVENT2:
            int2_count ++;
            break;
    }
    DIO_GetCOSLatchDataInt32(card, P7443_CH1, &CosData);
    printf("INT2 count = %d, COS Data: %x. \n", int2_count, CosData);
    return;
}

void sig3_handler(int signo)
{
    U32 CosData = 0;
    switch(signo){
        case SIGEVENT3:
            int3_count ++;
            break;
    }
    DIO_GetCOSLatchDataInt32(card, P7443_CH2, &CosData);
    printf("INT3 count = %d, COS Data: %x. \n", int3_count, CosData);
    return;
}

void sig4_handler(int signo)
{
    U32 CosData = 0;
    switch(signo){
        case SIGEVENT4:
            int4_count ++;
            break;
    }
    DIO_GetCOSLatchDataInt32(card, P7443_CH3, &CosData);
    printf("INT4 count = %d, COS Data: %x. \n", int4_count, CosData);
    return;
}

int main(void)
{
    int  err;
    char temp_char;

    setbuf( stdout, NULL );
    printf("\nPCI-7443 Dual Interrupt Sample\n" );
    printf("\nThis program shows the number of occurrence of interrupt\n" );
    printf("for COS0, COS1, COS2 and COS3 interrupt lines.\n" );

    printf("\nPlease input a card number: ");
    scanf(" %hd", &card_number);

    if((card=Register_Card(PCI_7443, card_number))<0){
        printf("Register_Card Error = %d\n", card);
        exit(0);
    }

    usleep(1000);

    err = DIO_SetCOSInterrupt32(card, P7443_CH0, 0x1, sig1_handler);
    if(err!=NoError){
        printf(" error from DIO_SetCOSInterrupt32 : %d \n" , err );
        goto exit_after_error;
    }
    err = DIO_SetCOSInterrupt32(card, P7443_CH1, 0x1, sig2_handler);
    if(err!=NoError){
        printf(" error from DIO_SetCOSInterrupt32 : %d \n" , err );
        goto exit_after_error;
    }

    err = DIO_SetCOSInterrupt32(card, P7443_CH2, 0x1, sig3_handler);
    if(err!=NoError){
        printf(" error from DIO_SetCOSInterrupt32 : %d \n" , err );
        goto exit_after_error;
    }

    err = DIO_SetCOSInterrupt32(card, P7443_CH3, 0x1, sig4_handler);
    if(err!=NoError){
        printf(" error from DIO_SetCOSInterrupt32 : %d \n" , err );
        goto exit_after_error;
    }

    printf("\nPress any key to exit...\n" );
    temp_char = getch();

    DIO_SetCOSInterrupt32(card, P7443_CH0, 0, /*sig1_handler*/NULL);
    DIO_SetCOSInterrupt32(card, P7443_CH1, 0, /*sig2_handler*/NULL);
    DIO_SetCOSInterrupt32(card, P7443_CH2, 0, /*sig2_handler*/NULL);
    DIO_SetCOSInterrupt32(card, P7443_CH3, 0, /*sig3_handler*/NULL);

exit_after_error:
    if(card>=0)
        Release_Card(card);
    exit(0);
}
