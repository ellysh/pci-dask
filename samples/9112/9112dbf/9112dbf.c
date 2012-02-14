#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "conio.h"
#include "dask.h"

U16 channel=3;  //4 channels
U16 range=AD_B_5_V;
I16 card = -1;
char *file_name="9112";
U32 read_count=2000;
F64 sample_rate=2000;
pthread_t thread1;
BOOLEAN clear_op;

void * ProcessThread( void * temp_pt )
{
    U32 count=0, count1;
    BOOLEAN halfReady, fStop;

    printf("\n\nData count : \n");
    do {
        do {
             AI_AsyncDblBufferHalfReady(card, &halfReady, &fStop);
        } while (!halfReady);
        AI_AsyncDblBufferTransfer(card, NULL);  //transfer to file
        count += (read_count / 2);
        printf("%ld\r", count);
    } while(!clear_op);
    AI_AsyncClear(card, &count1);

    return NULL;
}


int main(void)
{
    I16 err;
    void * retval;

    setbuf( stdout, NULL );
    clrscr();
    printf("This program inputs data from CH-%d to CH-0 of PCI-9112 in %d Hz by\ndouble-buffer mode, and store data to file '%s.dat'. The size of circular\nbuffer is %ld. It will not stop until you press a key.\n\nPress any key to start the operation.\n", channel, (int)sample_rate, file_name, read_count);
    getch();
    if ((card=Register_Card (PCI_9112, 0)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }
    err = AI_9112_Config(card, TRIG_INT_PACER);
    if (err!=0) {
        printf("AI_9112_Config error=%d", err);
        Release_Card( card );
        exit(1);
    }
    err = AI_AsyncDblBufferMode(card, 1);  //double-buffer mode
    if (err!=0) {
        printf("AI_DblBufferMode error=%d", err);
        Release_Card( card );
        exit(1);
    }
    err = AI_ContScanChannelsToFile(card, channel, range, file_name, read_count, sample_rate, ASYNCH_OP);
    if (err!=0) {
        printf("AI_ContReadChannel error=%d", err);
        Release_Card( card );
        exit(1);
    }
    clear_op = FALSE;

    printf("\n\n\n\n\n Data conversion start , Press any key to stop input operation.\n");

    if( pthread_create( &thread1, NULL, ProcessThread, NULL ) != 0 ){
        printf(" Can't create thread1 , terminate \n ");
        Release_Card(card);
        exit(-1);
    }

    getch();
    clear_op = TRUE;

    pthread_join(thread1, &retval);
    Release_Card(card);

    exit(0);
}
