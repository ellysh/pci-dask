#include <stdio.h>
#include "dask.h"
#include "conio.h"

int main(void)
{
    I16 card = -1, card_number = 0;
    int ch;
    F64 out_value;
    char c;

    setbuf(stdout, NULL);

    if((card = Register_Card(PCI_6202, card_number))<0){
        printf ("Can't open device file: PCI6202\n");
        exit(-1);
    }

    while(1){
        clrscr();
        printf("PCI-6202 AO Sample :\n\n");
        printf("-------------------------------------------------\n");
        printf("Please type the channel number [0-3]: ");
        scanf(" %d", &ch);
        if((ch<0)||(ch>3)){
            printf("The channel number is invalid\n");
            goto endwhile;
        }
        printf("Please specify a voltage to output (-10V~10V): ");
        scanf(" %lf", &out_value);
        if((out_value<-10)||(out_value>10)){
            printf("The voltage value is invalid\n");
            goto endwhile;
        }
        printf("The output value is %lf .\n", out_value);
        AO_VWriteChannel(card, ch, out_value);
        printf ("-------------------------------------------------\n");
endwhile:
        printf ("[q]Quit [c]Continue ? ");
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
