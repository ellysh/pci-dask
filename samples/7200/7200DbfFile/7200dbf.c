#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include "dask.h"
#include "conio.h"

char *file_name = "7200d";
U32 DoBuf[2000];
U32 read_count = 2000;
F64 sample_rate = 1000;
I16 card = -1;
BOOLEAN clear_op;

void* ProcessThread(void *temp_pt)
{
    BOOLEAN halfReady;

    U32 count = 0, count1;

    do{
        do{
            DI_AsyncDblBufferHalfReady(card, &halfReady);
        }while(!halfReady);
        DI_AsyncDblBufferTransfer(card, NULL); //transfer to file
        count += (read_count/2);
        printf("%li\r", count);
    }while(!clear_op);

    printf("\n\n%li input data are stored in file '%s.dat'.\n", count, file_name);

    DI_AsyncClear(card, &count);
    DO_AsyncClear(card, &count1);

    return NULL;
}


int main(void)
{
    I16 err, card_num = 0;
    U32 i;
    pthread_t thread1;
    void *retval;

    setbuf(stdout, NULL);
    printf("This program inputs %ld data by PCI-7200 in %li Hz by double-buffer mode, and store data to file '%s.dat'. It will not stop until you press a key.\n\nPress any key to start the operation.\n",
        read_count, (unsigned long)sample_rate, file_name);
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
    err = DI_AsyncDblBufferMode(card, 1);
    if(err!=0){
        printf("DI_DblBufferMode error=%d", err);
        Release_Card(card);
        exit(1);
    }

    err = DO_ContWritePort(card, 0, DoBuf, read_count, 0, (F64)sample_rate, ASYNCH_OP);
    if(err!=0){
        printf("DO_ContWritePort error=%d", err);
    }
    err = DI_ContReadPortToFile(card, 0, file_name, read_count, sample_rate, ASYNCH_OP);
    if(err!=0){
        printf("DI_ContReadPort error=%d", err);
        Release_Card(card);
        exit(1);
    }
    printf("\n\nPress any key to stop input operation.");
    printf("\n\nData count : \n");

    clear_op = FALSE;

    if(pthread_create(&thread1, NULL, ProcessThread, NULL)!=0){
        printf(" Can't create thread1 , terminate \n ");
        Release_Card(card);
        exit(-1);
    }

    getch();
    clear_op = TRUE;
    pthread_join(thread1, &retval);
    Release_Card(card);

    printf("\nPress ENTER to exit the program. \n"); getch();
    exit(1);
}
