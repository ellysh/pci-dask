#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "dask.h"
#include "conio.h"

F64 sample_rate=10000;
U16 channel=3, range = AD_B_1_V;
BOOLEAN fstop =0;
U32 read_count=10000, mem_size;
I16 *ai_buf;
I16 card=-1;
U32 count;
U16 card_type = 0;
char *file_name="9812dat.txt";
FILE* fp;

void ProcessData(void);
void show_channel_data(void);

int main(void)
{
    I16 err;

    setbuf( stdout, NULL );
    if ( (card = Register_Card(PCI_9812, 0)) < 0) {
        printf("Register_Card error=%d", card);
        exit(1);
    }
    err = AI_9812_Config(card,P9812_TRGMOD_POST, P9812_TRGSRC_EXT_DIG, P9812_TRGSLP_POS, P9812_AD2_GT_PCI|P9812_CLKSRC_INT,0x80,0x10);
    if( err != NoError){
        printf("9812 : AI_Config Error = %d\n", err);
        Release_Card(card);
        exit(1);
    }

    if (( fp = fopen( file_name , "w+b")) == NULL){
        Release_Card(card);
        exit(1);
    }

    mem_size=read_count * 2;
    ai_buf = (I16*)malloc(mem_size);
    err = AI_ContScanChannels (card, channel, range, ai_buf, read_count, sample_rate, ASYNCH_OP);
    if( err != NoError) {
        printf(" 9812 : AI_ContReadPort Error = %d\n", err);
        Release_Card(card);
        free( ai_buf );
        fclose( fp );
        exit(1);
    }

    printf(" ContScanChannels with Post Trigger operation \n");
    printf(" Start the Data consversion with the External Trigger signal \n" );

    fstop = 0;
    do{
        AI_AsyncCheck(card, &fstop, &count);
    }while (!fstop);
    AI_AsyncClear(card, &count); // stop data aquisition & memory copy

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

    fprintf( fp, "ContScanChannels with Post Trigger operation \n");
    fprintf( fp, "count_per_page:%li\n", count );
    fprintf( fp, "channel_num:%d\n", num_chan );
    fprintf( fp, "sample_rate(Hz):%li\n", (U32)sample_rate );
    fprintf( fp, "CH0\n======================\n");

    for (i=0; i<count; i+=num_chan) {
        s[0] = 0x00;
        sprintf( s, "%04x  %04x  %04x  %04x  \n", (U16)ai_buf[i], (U16)ai_buf[i+1], (U16)ai_buf[i+2], (U16)ai_buf[i+3]);
        fwrite(s, sizeof(U8), strlen(s), fp);
    }
    fclose( fp );

}

void show_channel_data(void)
{
    U16 adinput;
    int channel_no, k, num_chan;
    int chan_idx;
    U32 i;

    num_chan = channel+1;
    for( chan_idx = 0; chan_idx<= channel; chan_idx++ ){
        printf(" >>>>>>>>>>>>>>> the first 240 data from Channel#%d  <<<<<<<<<<<<<<< \n", chan_idx+1 );
        for( i = 0, k=0; i< read_count && k< 240 ; i++ ){
            adinput = (U16)ai_buf[i];
            channel_no = i%num_chan;
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
