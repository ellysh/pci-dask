#include <stdio.h>
#include "dask.h"
#include "conio.h"

int main( void )
{
    I16 card=-1, card_number = 0;
    long out_value;
    int ch;
    float volt;
    char c;

    setbuf(stdout, NULL);

    if((card = Register_Card(PCI_6208A, card_number))<0){
        printf("Can't open device file: PCI6208A\n");
        exit(-1);
    }

    while(1){
        clrscr();
        printf("PCI-6208A AO Sample :\n\n");
        printf("-------------------------------------------------\n");
        printf("Please type the channel number [0-7]: ");
        scanf(" %d", &ch);
        if((ch<0)||(ch>7)){
            printf("The channel number is invalid\n");
            goto endwhile;
        }
        printf("Please specify a current value to output (4mA ~20mA): ");
        scanf(" %f", &volt);
        printf("%f\n", volt);
        volt = (volt-4)/16;
        out_value = 32767*volt;
        if(out_value>32767)
            out_value = 0x7fff;
        if(out_value<0)
            out_value = 0;
        printf("The output value is %ld.\n", out_value);
        AO_6208A_Config(card, P6208_CURRENT_4_20MA);
        AO_WriteChannel(card, ch, out_value);
        printf("-------------------------------------------------\n");
endwhile:
        printf("[q]Quit [c]Continue ? ");
        //scanf(" %c", &c);
        c = getch();
        if((c=='q')||(c =='Q'))
            break;
    }

    if(card>=0){
       Release_Card(card);
    }
    return 0;
}
