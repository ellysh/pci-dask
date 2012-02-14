#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "dask.h"
#include "conio.h"

F64 sample_rate=30000;
U16 channel;
BOOLEAN fstop;
U32 read_count=10000, mem_size;
char *file_name="9118dat.txt";
U16 *ai_buf;
I16 card=-1;
U32 count, postCount=512;
FILE* fp;

void ProcessData(void);
void show_channel_data(void);

int main( void )
{
    I16 err;
    int i;

    setbuf( stdout, NULL );
    if ( (card = Register_Card(PCI_9118DG, 0)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

    if (read_count <= postCount) {
        printf("Buffer Size have to be larger than %ld\n", postCount+2);
        Release_Card(card);
        exit(1);
    }

    err=AI_9118_Config(card,P9118_AI_BiPolar|P9118_AI_SingEnded,P9118_AI_DtrgPositive|P9118_AI_EtrgPositive|P9118_AI_PostTrgEn,0,postCount);
    if( err != NoError) {
        printf("AI_Config Error = %d\n", err);
        Release_Card(card);
        exit(1);
    }

    if (( fp = fopen( file_name , "w+b")) == NULL){
        Release_Card(card);
        exit(1);
    }

    mem_size=read_count * 2;
    ai_buf = (U16*)malloc(mem_size);

    for (i=0;i<read_count;i++)
        ai_buf[i]=0xffff;
    fstop =0;

    err=AI_ContReadChannel(card, channel, AD_B_5_V, ai_buf, read_count, sample_rate, ASYNCH_OP);
    if( err != NoError) {
        printf("AI_ContReadPort Error = %d\n", err);
        Release_Card(card);
        free( ai_buf );
        fclose( fp );
        exit(1);
    }

    do {
        AI_AsyncCheck(card, &fstop, &count);
    }while (!fstop);
    AI_AsyncClear(card, &count);

    ProcessData();
    show_channel_data();
    printf(" the data is saved into %s\n", file_name );
    fclose( fp );
    free( ai_buf );

    return 5;
}


void ProcessData()
{

    int num_chan = channel + 1;
    int i;
    char s[100];

    fprintf( fp, " ContRcanChannel with Post Trigger operation \n");
    fprintf( fp, "count_per_page:%li\n", count );
    fprintf( fp, "channel_num:%d\n", num_chan );
    fprintf( fp, "sample_rate(Hz):%li\n", (U32)sample_rate );
    fprintf( fp, "CH0\n======================\n");

    for (i=0; i<count; i++) {
        //adinput = ai_buf[i]>>4;
        s[0] = 0x00;
        sprintf( s, "%04x  \n", ai_buf[i]);
        fwrite(s, sizeof(U8), strlen(s), fp);
    }

}


void show_channel_data()
{
    U16 adinput;
    int channel_no, k;
    int chan_idx;
    U32 i;

    for( chan_idx = 0; chan_idx<= channel; chan_idx++ ){
        printf(" >>>>>>>>>>>>>>> the first 240 data from Channel#%d  <<<<<<<<<<<<<<< \n", chan_idx+1 );
        for( i = 0, k=0; i< read_count && k< 240 ; i++ ){
            adinput = (U16)ai_buf[i];
            channel_no = adinput & 0x000f;
            adinput = adinput >>4;

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
