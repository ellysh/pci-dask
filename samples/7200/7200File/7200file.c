#include <stdio.h>
#include <errno.h>
#include "dask.h"
#include "conio.h"

U32 DoBuf[2000];
char *file_name = "7200d";
U32 read_count = 2000;
F64 sample_rate = 10000;

int main(void)
{
    I16 card, err, card_num = 0;
    BOOLEAN bStopped;
    U32 count, i;

    setbuf(stdout, NULL);

    printf("This program inputs and outputs %ld data by PCI-7200 in %d Hz simultaneously, and store input data to file '%s.dat'.\n\nPress any key to start the operation.\n", read_count, (int)sample_rate, file_name);
    getch();

    if((card = Register_Card(PCI_7200, card_num))<0){
        printf("Register_Card error=%d\n", card);
        exit(1);
    }

    for(i=0; i<read_count; i++)
        //DoBuf[i] = (U32)(sin((double)i/500)*0x7fff)+0x8000;
        DoBuf[i] = i;

    err = DI_7200_Config(card, TRIG_INT_PACER, DI_NOWAITING, DI_TRIG_FALLING, IREQ_FALLING);
    if(err!=0){
       printf("DI_7200_Config error=%d", err);
       Release_Card(card);
       exit(1);
    }

    err = DO_7200_Config(card, TRIG_INT_PACER, OREQ_DISABLE, OTRIG_LOW);
    if(err!=0){
       printf("DO_7200_Config error=%d", err);
       Release_Card(card);
       exit(1);
    }

    err = DI_AsyncDblBufferMode(card, 0);
    if(err!=0){
       printf("DI_DblBufferMode error=%d", err);
       Release_Card(card);
       exit(1);
    }

    err = DO_ContWritePort(card, 0, DoBuf, read_count, 1, (F64)sample_rate, ASYNCH_OP);
    if(err!=0){
       printf("DO_ContWritePort error=%d", err);
    }

    err = DI_ContReadPortToFile(card, 0, file_name, read_count, (F64)sample_rate, ASYNCH_OP);
    if(err!=0){
       printf("DI_ContReadPort error=%d", err);
       Release_Card(card);
       exit(1);
    }

    do{
        DI_AsyncCheck(card, &bStopped, &count);
    }while (!bStopped);

    DI_AsyncClear(card, &count);

    DO_AsyncClear(card, &count);

    Release_Card(card);

    printf("\n\nThe input data is already stored in file '%s.dat'.\n", file_name);
    printf("\nPress ENTER to exit the program. \n"); getch();
    exit(1);
}
