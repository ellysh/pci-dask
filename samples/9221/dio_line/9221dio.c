#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 Port = 0;
    U16 Line = 0;
    U16 out_data = 0;
    U16 rb_data;
    U16 read_data;


    printf("This sample outputs digital value to DO Line0, \n");
    printf("and reads DI Line0. (Connect pin3 and pin8)\n\n");
    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if((card=Register_Card(PCI_9221, card_num))<0){
        printf("Register_Card error=%d", card);
        exit(1);
    }

    do{
        clrscr();

        //Write Data to DO Line
        err = DO_WriteLine(card, Port, Line, out_data);
        if(err<0){
            printf("DO Write Line0 Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        printf("DO Write Data to Line0: %d\n", out_data);

        //Read Back Data from DO Line
        err = DO_ReadLine(card, Port, Line, &rb_data);
        if(err<0){
            printf("DO Read Line0 Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        printf("DO Read Back from Line0: %d\n", rb_data);

        //Read Data from DI Line
        err = DI_ReadLine(card, Port, Line, &read_data);
        if(err<0){
            printf("DI Read Line0 Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        printf("DI Read Data from Line0: %d\n", read_data);

        out_data = (++out_data)%2;

        printf("Press Enter to exit...\n");
        usleep(200000);
    }while(!kbhit());

    Release_Card(card);
    return 0;
}
