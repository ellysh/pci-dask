#include <stdio.h>
#include <stdlib.h>
#include "dask.h"
#include "conio.h"

F64 sample_rate=10000, samp_intrv=240;
U16 channel=3, range = AD_B_5_V;
U32 read_count=10240, mem_size;
char *file_name="9116dat.txt";
BOOLEAN fstop =0;
I16  *ai_buf;
int enM;
I16 card=-1, card_number = 0;
U32 count, postCount= 1024;
FILE* fp;

void ProcessData(void);
void show_channel_data(void);

int main(void)
{
    I16 err;
    int trg_mode, mCount = 100;

    setbuf( stdout, NULL );
    printf("Please select the trigger mode (0:Middle Trigger  1:Pre-Trigger): ");
    scanf(" %d", &trg_mode);
    printf("Enable MCounter? (0:no  1:yes): ");
    scanf(" %d", &enM);
    if(enM) {
        printf("MCount counter value: ");
        scanf(" %d", &mCount);
    }
    if ( (card = Register_Card(PCI_9116, 0)) < 0) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

    if(!enM)
        err = AI_9116_Config(card,0,P9116_AI_DMA|(trg_mode?P9116_TRGMOD_PRE:P9116_TRGMOD_MIDL), (trg_mode?0:postCount),0,0);
    else
        err = AI_9116_Config(card,0,P9116_AI_DMA|(trg_mode?P9116_TRGMOD_PRE:P9116_TRGMOD_MIDL)|P9116_AI_MCounterEn, (trg_mode?0:postCount),mCount,0);

    err = AI_9116_CounterInterval (card, samp_intrv*(channel+1), samp_intrv);

    if (( fp = fopen( file_name , "w+b")) == NULL){
        Release_Card(card);
        exit(1);
    }

    if(enM)
        read_count += mCount;

    mem_size=read_count * 2;
    ai_buf = (I16*)malloc(mem_size);
    err = AI_ContScanChannels(card, channel, range, ai_buf, read_count/(channel+1), (F64)1, ASYNCH_OP);
    if( err != NoError) {
        printf(" 9118 : AI_ContReadPort Error = %d\n", err);
        Release_Card(card);
        free( ai_buf );
        fclose( fp );
        exit(1);
    }
    printf( "ContScanChannels with Middle Trigger operation \n");
    printf(" About the Data consversion with the External Trigger signal \n" );

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

    fprintf( fp, "ContScanChannels with Middle Trigger operation \n");
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
            adinput = adinput;

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
