#include <stdio.h>
#include "dask.h"
#include "conio.h"

int main(void)
{
    I16 card = -1, card_number = 0;
    I16 out_value;
    int ch;
    float volt;
    char c;

    setbuf(stdout, NULL);

    if((card = Register_Card(PCI_6308A, card_number))<0){
        printf("Can't open device file: PCI6308A\n");
        exit(-1);
    }

    while(1){
        clrscr();
        printf("PCI-6308A AO Sample :\n\n");
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
        printf("volt = %f", volt);
        out_value = (I16)(4095*volt);
        if(out_value>4095)
            out_value = 4095;
        if(out_value<0)
            out_value = 0;
        printf("The output value is %d.\n", out_value);
        AO_6308A_Config(card, P6308_CURRENT_4_20MA);
        AO_WriteChannel(card, ch, out_value);
        printf("-------------------------------------------------\n");
endwhile:
        printf("[q]Quit [c]Continue ? ");
        //scanf(" %c", &c);
        c = getch();
        if((c=='q')||(c=='Q'))
            break;
    }
    putchar('\n');

    if(card>=0){
       Release_Card(card);
    }
    return 0;
}
