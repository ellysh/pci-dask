#include <stdio.h>
#include "dask.h"
#include "conio.h"

int main(void)
{
    I16 card = -1, card_number = 0;
    int ch;
    long out_value;
    int volt1;
    I16 volt;
    char c;

    setbuf(stdout, NULL);
    if((card = Register_Card(PCI_6208V, card_number))<0){
        printf("Can't open device file: PCI6208V\n");
        exit(-1);
    }

    while(1){
        clrscr();
        printf("PCI-6208V AO Sample :\n\n");
        printf("-------------------------------------------------\n");
        printf("Please type the channel number [0-15]: ");
        scanf(" %d", &ch);
        if((ch<0)||(ch>15)){
            printf("The channel number is invalid\n");
            goto endwhile;
        }
        printf("Please specify a voltage to output (-10V~10V): ");
        scanf(" %d", &volt1);
        volt = (I16)volt1;
        if((volt<-10)||(volt>10)){
            printf("The voltage value is invalid\n");
            goto endwhile;
        }
        out_value = (65536/20)*volt;
        if(out_value>32767)
            out_value = 0x7fff;
        if(out_value<-32768)
            out_value = 0x8000;
        printf("The output value is %ld.\n", out_value);
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
