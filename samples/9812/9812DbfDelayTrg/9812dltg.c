#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "dask.h"
#include "conio.h"

unsigned int sample_rate=10000;
U16 channel = 3;
U16 range=AD_B_1_V;
BOOLEAN fStop, HalfReady;
U32 read_count=10240, mem_size;
I16  *ai_buf;
I16 card=-1, card_number = 0;
U32 count, postCount = 1024;
BOOLEAN clear_op;
pthread_t thread1;
char *file_name="9812dat.txt";
FILE* fp = NULL;

void write_to_file( U16 *buf, int num_chan )
{
    U8 s[100];

    U32 i, half_count;

    half_count = read_count /2 ;

    for(i=0; i<half_count; i+=num_chan){
        s[0] = 0x00;
        sprintf( s, "%04x  %04x  %04x  %04x\n", buf[i], buf[i+1], buf[i+2], buf[i+3] );
        fwrite(s, sizeof(U8), strlen(s), fp);
    }
}

void * ProcessThread( void * temp_pt )
{
    BOOLEAN HalfReady;

    U32 count=0, count1;
    U32 half_count;

    half_count = read_count / 2;
    fprintf( fp, "Double Buffer Transfer With Delay Trigger operation\n");
    fprintf( fp, "count_per_page:%li\n", half_count );
    fprintf( fp, "channel_num:%d\n", channel+1 );
    fprintf( fp, "sample_rate(Hz):%li\n", (U32)sample_rate );
    fprintf( fp, "CH0   CH1   CH2   CH3 \n======================\n");


    do {
        do {
            AI_AsyncDblBufferHalfReady(card, &HalfReady, &fStop);
        } while (!HalfReady & !fStop);

        if (!fStop) {
            AI_AsyncDblBufferTransfer(card, ai_buf);
            count += half_count;
            write_to_file( ai_buf, channel+1);
        }
    } while( !clear_op );

    AI_AsyncClear(card, &count1);
    //fclose( fp );

    printf(" Data conversion stop\n");
    printf(" %ld input data are stored in file '%s'.\n\n\n\n", count, file_name);

    Release_Card(card);

    return NULL;
}


int main(void)
{
    I16 err;

    setbuf( stdout, NULL );
    printf("Double Buffer Transfer With Delay Trigger operation\n");
    printf("This program inputs data from CH-0 to CH-%d of PCI-9812 in %d Hz by\ndouble-buffer mode, and store data to file '%s'. The size of circular\nbuffer is %ld. It will not stop until you press a key.\n\n",
        channel, (int)sample_rate, file_name, read_count);

    if ((card=Register_Card (PCI_9812, 0)) <0 ) {
        printf("Register_Card error=%d\n", card);
        exit(1);
    }

    err = AI_9812_Config(card,P9812_TRGMOD_DELAY, P9812_TRGSRC_EXT_DIG, P9812_TRGSLP_POS, P9812_AD2_GT_PCI|P9812_CLKSRC_INT,0x80,postCount);
    if (err!=0) {
        printf("AI_9812_Config error=%d", err);
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

    mem_size=read_count * 2;
    ai_buf = (U16*)malloc(mem_size);

    err = AI_ContScanChannels(card, channel, range, ai_buf, read_count, sample_rate, ASYNCH_OP);
    if (err!=0) {
        printf("AI_ContScanChannels error=%d", err);
        free( ai_buf );
        exit(1);
    }

    clear_op = FALSE;

    printf("\n\n\n Start Data Conversion by External Trigger Signal and Press any key to stop.\n");

    if( pthread_create( &thread1, NULL, ProcessThread, NULL ) != 0 ){
        printf(" Can't create thread1 , terminate \n ");
        Release_Card(card);
        free( ai_buf );
        fclose( fp );
        exit(-1);
    }

    getch();
    clear_op = TRUE;
    sleep(1);
    printf("\n\n                                    Press any key to exit the program..... "); getch();
    free( ai_buf );
    fclose( fp );
    return 0;
}
