#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "dask.h"
#include "conio.h"

F64  sample_rate=300000;
I16 card = -1, card_number = 0, burst_num = 4;
long  read_count=40960;
char *file_name="9118dat.txt";
U16   channel=3, range = AD_B_5_V;
U32  mem_size = 0;
U16 *ai_buf;
FILE *fp;

void show_channel_data(void);

int main( void )
{
    I16 err;
    int i;
    char s[100];
    int num_chan = (int)channel + 1;

    setbuf( stdout, NULL );
    printf("This program inputs data from CH-0 to CH-%d of PCI-9118 in %d Hz by\nBurst DMA SYNCH_OP mode, and store data to file '%s'.\nThe size of circular buffer is %ld.\n", channel, (int)sample_rate, file_name, read_count);
    if ((card=Register_Card (PCI_9118DG, 0)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

    err = AI_9118_Config(card, P9118_AI_BiPolar|P9118_AI_SingEnded,P9118_AI_BurstModeEn|P9118_AI_SampleHold,burst_num,0);
    if (err!=0) {
        printf("AI_9118_Config error=%d", err);
        exit(1);
    }

    err = AI_AsyncDblBufferMode(card, 0);  // None double-buffer mode
    if (err!=0) {
        printf("AI_DblBufferMode error=%d", err);
        exit(1);
    }

    if (( fp = fopen( file_name , "w+b")) == NULL){
        Release_Card(card);
        exit(1);
    }

    mem_size=read_count * 2;
    ai_buf = (U16*)malloc(mem_size);

    err = AI_ContScanChannels(card, channel, range, ai_buf, read_count, sample_rate, SYNCH_OP);
    if (err!=0) {
        printf("AI_ContReadChannel error=%d", err);
        free( ai_buf );
        exit(1);
    }


    fprintf( fp, "ContScanChannels with Burst DMA operation \n");
    fprintf( fp, "count_per_page:%li\n", read_count );
    fprintf( fp, "channel_num:%d\n", channel+1 );
    fprintf( fp, "sample_rate(Hz):%li\n", (U32)sample_rate );
    fprintf( fp, "CH0   CH1   CH2   CH3 \n======================\n");
    // information for showdata
    for(i=0; i<read_count; i+=num_chan){
        //buf[i+j] >> 4;
        s[0] = 0x00;
        sprintf( s, "%04x  %04x  %04x  %04x\n", ai_buf[i], ai_buf[i+1], ai_buf[i+2], ai_buf[i+3]);
        fwrite(s, sizeof(U8), strlen(s), fp);
    }
    fclose( fp );

    show_channel_data();

    printf("\n\n                                    Press any key to exit the program..... "); getch();
    free( ai_buf );

    return 0;
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
