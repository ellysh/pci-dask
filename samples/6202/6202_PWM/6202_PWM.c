#include <stdio.h>
#include "dask.h"
#include "conio.h"

int main(void)
{
    I16 card = -1, card_number = 0;
    int ch;
    U32 high_interval, low_interval;
    char c;

    setbuf(stdout, NULL);

    if((card = Register_Card(PCI_6202, card_number))<0){
        printf("Register_Card %d error: %d", card_number, card);
        exit(-1);
    }

    while(1){
        clrscr();
        printf("PCI-6202 PWM Sample :\n\n");
        printf("-------------------------------------------------\n");
        printf("Please type the channel number [0-3]: ");
        scanf(" %d", &ch);
        if((ch<0)||(ch>3)){
            printf("The channel number is invalid\n");
            goto endwhile;
        }
        printf("Please specify high-intervel of PWM (1~16777215): ");
        scanf(" %d", &high_interval);
        if((high_interval<1)||(high_interval>16777215)){
            printf("The value is invalid\n");
            goto endwhile;
        }
        printf("Please specify low-intervel of PWM (1~16777215): ");
        scanf(" %d", &low_interval);
        if((low_interval<1)||(low_interval>16777215)){
            printf("The value is invalid\n");
            goto endwhile;
        }
        printf("PWM Start!\n");
        PWM_Output(card, ch, high_interval, low_interval);
        printf("-------------------------------------------------\n\n");
        printf("Press any key to Stop PWM!\n");
        getch();
        PWM_Stop(card, ch);
endwhile:
        printf("[q]Quit [c]Continue ? ");
        //scanf(" %c", &c);
        c = getch();
        if((c=='q')||(c =='Q'))
            break;
    }
    putchar('\n');

    if(card>=0){
        Release_Card(card);
    }
    return 0;
}
