#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "dask.h"
#include "conio.h"

unsigned int sample_rate=100000, samp_intrv=240;
U16 channel = 3;
U16 range=AD_B_5_V;
BOOLEAN fStop, HalfReady;
U32 read_count=1024, mem_size;
I16  *ai_buf;
I16 card=-1;
U32 count;
BOOLEAN clear_op;
pthread_t thread1;
char *file_name="9116dat.txt";
FILE* fp = NULL;

void write_to_file( U16 *buf, int num_chan, U32 write_count )
{
    U8 s[100];

    U32 i;

    for(i=0; i<write_count; i+=num_chan){
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
    fprintf( fp, "Double Buffer Transfer With Post Trigger operation\n");
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
            write_to_file( ai_buf, channel+1, half_count );
        }else break;

    } while( !clear_op );

    AI_AsyncDblBufferHalfReady(card, &HalfReady, &fStop);  //Get the index of working buffer of Double Buffer
    AI_AsyncClear(card, &count1);
    if (!HalfReady && ((count1 == half_count)||(count1 == 0))) {
    }
    else {
        if (count1 ==0)
            count1 = half_count;
        AI_AsyncDblBufferTransfer(card, ai_buf);
        count += count1;
        write_to_file( ai_buf, channel+1, count1);
    }

    fclose( fp );

    printf(" Data conversion stop\n");
    printf(" %ld input data are stored in file '%s'.\n\n\n\n", count, file_name);

    Release_Card(card);
    if( fStop)
        printf("\n\n                                Press any any key to exit the program..... "); // key capture in main()

    return NULL;
}


int main(void)
{
    I16 err;
    int ai_mode;

    setbuf( stdout, NULL );
    printf("Please select the AI type (0:DMA  1:INT): ");
    scanf(" %d", &ai_mode);
    printf("Double Buffer Transfer With Post Trigger operation\n");
    printf("This program inputs data from CH-0 to CH-%d of PCI-9116 in %d Hz by\ndouble-buffer mode, and store data to file '%s'. The size of circular\nbuffer is %ld. It will not stop until you press a key.\n\n",
        channel, (int)sample_rate, file_name, read_count);

    if ((card=Register_Card (PCI_9116, 0)) <0 ) {
        printf("Register_Card error=%d\n", card);
        exit(1);
    }
    if(!ai_mode)
        AI_9116_Config(card,0,P9116_AI_DMA|P9116_TRGMOD_POST,0,0,0);
    else
        AI_9116_Config(card,0,P9116_AI_INT|P9116_TRGMOD_POST,0,0,0);

    AI_9116_CounterInterval (card, samp_intrv*(channel+1), samp_intrv);

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
    err = AI_ContScanChannels(card, channel, range, ai_buf, read_count/(channel+1), (F64)1, ASYNCH_OP);
    if (err!=0) {
        printf("AI_ContScanChannels error=%d", err);
        free( ai_buf );
        exit(1);
    }

    clear_op = FALSE;
    fStop = FALSE;
    printf("\n\n\n Start Data Conversion by External Trigger Signal and Press any key to stop.\n");

    if( pthread_create( &thread1, NULL, ProcessThread, NULL ) != 0 ){
        printf(" Can't create thread1 , terminate \n ");
        Release_Card(card);
        free( ai_buf );
        fclose( fp );
        exit(-1);
    }

    getch();
    if( !fStop ){ // teminate by keyboard , not the trigger signal
        clear_op = TRUE;
        sleep(1);
        printf("\n\n                                    Press any ENTER to exit the program..... "); getchar();
    }
    else
        putchar('\n');
    free( ai_buf );
    fclose( fp );

    return 0;
}
