#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "dask.h"
#include "conio.h"

F64 sample_rate=10000;
U16 channel;
BOOLEAN fstop, HalfReady;
U32 read_count=1024, mem_size;
U16 *ai_buf;
char *file_name="9118dat.txt";
pthread_t thread1;
I16 card=-1;
U32 count, postCount = 1024;
BOOLEAN clear_op;
FILE* fp;

void * ProcessThread( void * temp_pt );
void exch_data(unsigned short * tbuf, U32 cnt);


int main( void )
{
    I16 err;

    setbuf( stdout, NULL );
    printf("PCI-9118 Double Buffered DMA with About Trigger Mode\n");

    if ((card=Register_Card (PCI_9118DG, 0)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }


    err=AI_9118_Config(card,P9118_AI_BiPolar|P9118_AI_SingEnded,P9118_AI_DtrgPositive|P9118_AI_EtrgPositive|P9118_AI_AboutTrgEn,0,postCount);
    if( err != NoError) {
        printf("AI_Config Error = %d\n", err);
        Release_Card(card);
        exit(1);
    }
    err=AI_AsyncDblBufferMode (card, 1);
    if( err != NoError) {
        printf("AI_AsyncDblBufferMode Error = %d\n", err);
        Release_Card(card);
        exit(1);
    }

    if (( fp = fopen( file_name , "w+b")) == NULL){
        Release_Card(card);
        exit(1);
    }

    mem_size=read_count * 2;
    ai_buf = (U16*)malloc(mem_size);

    err=AI_ContReadChannel(card, channel, AD_B_5_V, ai_buf, read_count, (F64)sample_rate, ASYNCH_OP);
    if( err != NoError) {
        printf("AI_ContReadPort Error = %d\n", err);
        free( ai_buf );
        Release_Card(card);
        exit(1);
    }
    fstop =0;
    clear_op = FALSE;

    fprintf( fp, "PCI-9118 Double Buffered DMA with About Trigger Mode\n");
    fprintf( fp, "count_per_page:%li\n", read_count );
    fprintf( fp, "channel_num:1\n" );
    fprintf( fp, "sample_rate(Hz):%li\n", (U32)sample_rate );
    fprintf( fp, "CH0\n======================\n");
    printf("\n\n\n Data conversion start , Input the Trigger Signal for About Trigger operation\n");

    if( pthread_create( &thread1, NULL, ProcessThread, NULL ) != 0 ){
        printf(" Can't create thread1 , terminate \n ");
        Release_Card(card);
        free( ai_buf );
        exit(-1);
    }

    getch();
    clear_op = TRUE;
    //sleep(1);
    free( ai_buf );
    return 0;
}


void * ProcessThread( void * temp_pt )
{
    BOOLEAN HalfReady;
    U32 i, half_count;
    char s[100];

    half_count = read_count/2;  //Every time transfer half size of buffer
    do {
        do {
           AI_AsyncDblBufferHalfReady(card, &HalfReady, &fstop);
        } while (!HalfReady && !fstop && !clear_op);

        if (!fstop) {
            AI_AsyncDblBufferTransfer(card, ai_buf);
            //exch_data(ai_buf, half_count);

            for (i=0; i<half_count; i++) {
                //adinput = ai_buf[i]>>4;
                s[0] = 0x00;
                sprintf( s, "%04x\n", ai_buf[i]);
                fwrite(s, sizeof(U8), strlen(s), fp);
            }
        }
    } while (!clear_op && !fstop);

    AI_AsyncDblBufferHalfReady(card, &HalfReady, &fstop);  //Get the index of working buffer of Double Buffer
    AI_AsyncClear(card, &count);
    if (!HalfReady && ((count == read_count)||(count == 0))) {
    }
    else {
        AI_AsyncDblBufferTransfer(card, ai_buf);
        if (count ==0)
            count = read_count;
        //exch_data(ai_buf, read_count);
        for (i=0; i<count; i++) {
            //adinput = ai_buf[i]>>4;
            s[0] = 0x00;
            sprintf( s, "%04x\n", ai_buf[i]);
            fwrite(s, sizeof(U8), strlen(s), fp);
        }
    }
    printf("\n\n\n Triggered by Extenal Trigger Signal, press any key to exit \n");
    fclose( fp );
    return NULL;
}

void exch_data(unsigned short * tbuf, U32 cnt)
{
    unsigned short temp;
    int i;
    for (i=0;i<cnt;i++) {
        if (!(i%2)) {
            temp = tbuf[i];
            tbuf[i] = tbuf[i+1];
            tbuf[i+1] = temp;
        }
    }
}
