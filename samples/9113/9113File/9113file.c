#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "dask.h"
#include "conio.h"

U16 channel=3;  //4 channels
U16 range=AD_B_5_V;
char *file_name="9113dat.txt";
BOOLEAN concat=FALSE;
U32 read_count=20480;
U32 mem_size;
U32 *ai_buf;
F64 sample_rate=10000;

BOOLEAN write_to_file( U32 *buf, int num_chan );
void show_channel_data( U32 *buf, int num_chan );

int main(void)
{
    I16 card, err;
    setbuf( stdout, NULL );
    printf("This program inputs %ld data from CH-0 to CH-%d of PCI-9113 in %d Hz, and\nstore data to file '%s'.\nPlease press any key to start the operation.", read_count, channel, (int)sample_rate, file_name);
    getch();
    if ((card=Register_Card (PCI_9113, 0)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

    err = AI_9113_Config(card, TRIG_INT_PACER);
    if (err!=0) {
        printf("AI_9111_Config error=%d", err);
        Release_Card(card);
        exit(1);
    }

    err = AI_AsyncDblBufferMode(card, 0);
    if (err!=0) {
        printf("AI_DblBufferMode error=%d", err);
        Release_Card(card);
        exit(1);
    }

    mem_size=read_count * 4;
    ai_buf = (I16*)malloc(mem_size);

    err = AI_ContScanChannels(card, channel, range, (U16*)ai_buf, read_count, sample_rate, SYNCH_OP);
    //err = AI_ContReadChannel(card, channel, range, (U16*)ai_buf, read_count, sample_rate, SYNCH_OP);
    if (err!=0) {
        printf("AI_ContReadChannel error=%d", err);
        Release_Card(card);
        free( ai_buf );
        exit(1);
    }

    Release_Card(card);

    if( write_to_file( ai_buf, channel+1 ) )
        printf("\n\nThe input data is already stored in file '%s'.\n", file_name);
    show_channel_data( ai_buf, channel+1 );
    free( ai_buf );
    printf("\nPress ENTER to exit the program. "); getchar();
    return 0;
}

BOOLEAN write_to_file( U32 *buf, int num_chan )
{
    U16 adinput[32];
    int chan_no;
    U8 s[100];
    U8 temp_s[16];
    FILE *fp;
    U32 i;
    int j;

    if (( fp = fopen( file_name , "w+b")) == NULL)
        return FALSE;

    fprintf( fp, "ContScanChannels with SYNCH_OP mode \n");
    fprintf( fp, "count_per_page:%li\n", read_count );
    fprintf( fp, "channel_num:%d\n", num_chan );
    fprintf( fp, "sample_rate(Hz):%li\n", (U32)sample_rate );
    fprintf( fp, "CH0   CH1   CH2   CH3 \n======================\n");

    for(i=0; i<read_count; i+=num_chan){
        for( j = 0; j < num_chan; j++ ){
            chan_no = ( buf[i+j]>>16) & 0x001f;
            adinput[chan_no] = buf[i+j] & 0x0fff;
            //adinput[j] = (U16)(buf[i+j] & 0x0fff);
        }

        s[0] = 0x00;

        for( j = 0; j < num_chan; j++ ){
            sprintf( temp_s, "%04x  ", adinput[j] );
            strcat( s, temp_s );
        }

        strcat(s, "\n");
        fwrite(s, sizeof(U8), strlen(s), fp);
    }

    fclose( fp );
    return TRUE;
}

void show_channel_data( U32 *buf, int num_chan )
{
    U16 adinput;
    int channel_no, chan_idx;
    U32 i, k;

    for( chan_idx = 0; chan_idx< num_chan; chan_idx++ ){
        printf(" >>>>>>>>>>>>>>> the first 240 data from Channel#%d  <<<<<<<<<<<<<<< \n", chan_idx+1 );
        for( i = 0, k=0; i< read_count && k< 240 ; i++ ){
            channel_no = (buf[i]>>16) & 0x001f;
            adinput = (U16)(buf[i] & 0x0fff);

            if( channel_no == chan_idx ){
                printf("%04x  ", adinput );
                if( (k+1)%12 == 0 )
                    putchar('\n');
                k++;
            }
        }

        if( chan_idx < num_chan -1 ){
            printf("press any key for next channel:\n");getch();
        }
    }
}
