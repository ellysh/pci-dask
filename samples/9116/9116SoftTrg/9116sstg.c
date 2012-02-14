#include <stdio.h>
#include <stdlib.h>
#include "dask.h"
#include "conio.h"

F64 sample_rate=10000, samp_intrv=480;
U16 channel=3, range = AD_B_5_V;
U32 read_count=10240, mem_size;
char *file_name="9116dat.txt";
BOOLEAN fstop =0;
I16  *ai_buf;
I16 card=-1, card_number = 0;
U32 count, postCount= 1024, MemSize;
FILE* fp;

void ProcessData(void);
void show_channel_data(void);

int main(void)
{
    I16 err;
    int ai_mode;

    setbuf( stdout, NULL );
    printf("Please select the AI type (0:DMA  1:INT): ");
    scanf(" %d", &ai_mode);

    if ( (card = Register_Card(PCI_9116, 0)) < 0) {
        printf("Register_Card error=%d", card);
        exit(1);
    }
    AI_InitialMemoryAllocated(card, &MemSize);
    printf("size: %lx \n", MemSize);
    if(!ai_mode)
        AI_9116_Config(card,0,P9116_AI_DMA,0,0,0);
    else
        AI_9116_Config(card,0,P9116_AI_INT,0,0,0);

    AI_9116_CounterInterval (card, samp_intrv*(channel+1), samp_intrv);

    if (( fp = fopen( file_name , "w+b")) == NULL){
        Release_Card(card);
        exit(1);
    }

    mem_size=read_count * 2;
    ai_buf = (I16*)malloc(mem_size);

    err = AI_ContScanChannels(card, channel, range, ai_buf, read_count/(channel+1), (F32)1, ASYNCH_OP);
    if( err != NoError) {
        printf(" 9118 : AI_ContReadPort Error = %d\n", err);
        Release_Card(card);
        free( ai_buf );
        fclose( fp );
        exit(1);
    }
    printf( "ContScanChannels with Soft Trigger operation \n");

    fstop = 0;
    do{
        AI_AsyncCheck(card, &fstop, &count);
    }while (!fstop);
    AI_AsyncClear(card, &count); // stop data aquisition & memory copy
    //getch();
    ProcessData();
    show_channel_data();
    free( ai_buf );
    printf(" the data is saved into %s\n", file_name );
    return 0;
}

void ProcessData(void)
{

    int num_chan = channel + 1;
    int i;
    char s[100];

    fprintf( fp, "ContRcanChannel with Soft Trigger operation\n");
    fprintf( fp, "count_per_page:%li\n", count );
    fprintf( fp, "channel_num:%d\n", num_chan );
    fprintf( fp, "sample_rate(Hz):%li\n", (U32)sample_rate );
    fprintf( fp, "CH0\n======================\n");

    for (i=0; i<count; i+=num_chan) {
        s[0] = 0x00;
        sprintf( s, "%04x  %04x  %04x  %04x  \n", (I16)ai_buf[i],(I16)ai_buf[i+1], (I16)ai_buf[i+2], (I16)ai_buf[i+3]);
        fwrite(s, sizeof(U8), strlen(s), fp);
    }

    fclose( fp );

}

void show_channel_data(void)
{
    I16 adinput;
    int channel_no, k, num_chan;
    int chan_idx;
    U32 i;

    num_chan = channel+1;
    for( chan_idx = 0; chan_idx<= channel; chan_idx++ ){
        printf(" >>>>>>>>>>>>>>> the first 240 data from Channel#%d  <<<<<<<<<<<<<<< \n", chan_idx+1 );
        for( i = 0, k=0; i< read_count && k< 240 ; i++ ){
            adinput = (I16)ai_buf[i];
            channel_no = i%num_chan;

            if( channel_no == chan_idx ){
                printf("%04x  ", adinput );
                if( (k+1)%12 == 0 )
                    putchar('\n');
                k++;
            }
        }

        if( chan_idx < channel ){
            printf("press any key for next channel:\n");getch();
        }
    }
}
