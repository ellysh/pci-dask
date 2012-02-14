#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "dask.h"
#include "conio.h"

U16 channel=3;  //4 channels
U16 range=AD_B_10_V;
I16 card;
char *file_name="9114dat.txt";
pthread_t thread1;
BOOLEAN concat=FALSE, fStop, clear_op;
U32 read_count=2048;
F64 sample_rate=2000;
U32 *ai_buf;
FILE *fp;


void write_to_file( U32 *buf, int num_chan )
{
    U16 adinput[32];
    int chan_no;
    U8 s[100];
    U8 temp_s[16];

    U32 i, half_count;
    int j;

    half_count = read_count /2 ;

    for(i=0; i<half_count; i+=num_chan){
        memset( adinput, 0xff, 32*sizeof(I16) );
        for( j = 0; j < num_chan; j++ ){
            chan_no = (buf[i+j]>>16) & 0x1f;
            adinput[chan_no] = (U16)(buf[i+j]&0xffff);
        }
        s[0] = 0x00;
        for( j = 0; j < num_chan; j++ ){
            sprintf( temp_s, "%04x  ", adinput[j] );
            strcat( s, temp_s );
        }
        strcat(s, "\n");
        fwrite(s, sizeof(U8), strlen(s), fp);
    }
}

void * ProcessThread( void * temp_pt )
{
    BOOLEAN HalfReady;

    U32 count=0, count1;

    fprintf( fp, " ContScanChannels with Double Buffer Transfer \n\n\n");
    fprintf( fp, "CH0   CH1   CH2   CH3 \n======================\n");

    do {
        do {
            AI_AsyncDblBufferHalfReady(card, &HalfReady, &fStop);
            usleep(10);
        } while (!HalfReady);
        if( !clear_op ){
            AI_AsyncDblBufferTransfer(card, ai_buf);
            count += (read_count / 2);
            write_to_file( ai_buf, channel+1 );
        }
    } while( !clear_op );

    fclose( fp );

    printf(" Data conversion stop\n");
    printf(" %ld input data are stored in file '%s'.\n\n\n\n", count, file_name);

    AI_AsyncClear(card, &count1);
    Release_Card(card);

    return NULL;
}


int main(void)
{
    I16 err;
    U32 mem_size;

    setbuf( stdout, NULL );
    clrscr();
    printf("\n 9114 Double Buffer Demo : \n");
    printf(" This program inputs data from CH-0 to CH-%d of PCI-9114 in %d Hz by\n double-buffer mode, and store data to file '%s'. The size of circular\n buffer is %ld. It will not stop until you press a key.\n\n Press any key to start the operation.\n", channel, (int)sample_rate, file_name,read_count);
    getch();
    if ((card=Register_Card (PCI_9114DG, 0)) <0 ) {
        printf("Register_Card error=%d", card);
        Release_Card(card);
        exit(1);
    }
    err = AI_9114_Config(card, TRIG_INT_PACER);
    if (err!=0) {
        printf("AI_9114_Config error=%d", err);
        Release_Card(card);
        exit(1);
    }
    err = AI_AsyncDblBufferMode(card, 1);  //double-buffer mode
    if (err!=0) {
        printf("AI_DblBufferMode error=%d", err);
        Release_Card(card);
        exit(1);
    }

    if (( fp = fopen( file_name , "w+b")) == NULL){
        Release_Card(card);
        exit(1);
    }

    mem_size=read_count * 4;
    ai_buf = (U32*)malloc(mem_size);

    err = AI_ContScanChannels(card, channel, range, ai_buf, read_count, sample_rate, ASYNCH_OP);
    if (err!=0) {
        printf("AI_ContReadChannel error=%d", err);
        Release_Card(card);
        free( ai_buf );
        fclose( fp );
        exit(1);
    }
    clear_op = FALSE;

    printf("\n\n\n\n\n Data conversion start , Press any key to stop input operation.\n");

    if( pthread_create( &thread1, NULL, ProcessThread, NULL ) != 0 ){
        printf(" Can't create thread1 , terminate \n ");
        Release_Card(card);
        free( ai_buf );
        exit(-1);
    }

    getch();
    clear_op = TRUE;
    sleep(1);
    printf("\n\n                                    Press any key to exit the program..... "); getch();
    free( ai_buf );

    return 0;
}
