#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // for open(...)
#include <sys/mman.h> // for mmap(...)
#include "dask.h"
#include "conio.h"

unsigned int sample_rate=2000;
U16 channel = 0;
U32 data_size=2048, mem_size;

char *file_name="9111dat.txt";
I16 *ai_buf;
I16 card=-1, card_number = 0;
U32 count, postCount = 512;
BOOLEAN clear_op, fstop;

BOOLEAN write_to_file( U16 *buf, int num_chan );
void show_channel_data( U16 *buf, int num_chan );

int main( void )
{
    I16 err;

    setbuf( stdout, NULL );
    if ((card=Register_Card(PCI_9111DG, card_number))< 0) {
        printf("Register_Card Error = %d\n", card);
        exit(-1);
    }

    clrscr();
    printf("*******************************************************************\n");
    printf("****                     9111 Pre-Trigger                     *****\n");
    printf("*******************************************************************\n\n");
    printf("This program inputs %ld data from CH-0 of PCI-9111 in %d Hz.\n\n", data_size,(int)sample_rate);
    printf("Please press any key to start the Pre-Trigger operation.");
    getch();

    if (data_size%512) {
        printf("Buffer Size should be devided by 512\n");
        Release_Card( card );
        exit(-1);
    }

    mem_size=data_size * 2;
    ai_buf = malloc(mem_size);
    fstop =0;

    if ((err=AI_9111_Config(card, TRIG_INT_PACER, 1, postCount))!= NoError) {
        printf(" IOcontrol:DAS_IOC_AI_CONFIG error : %d \n", err );
        Release_Card( card );
        free( ai_buf );
        exit(-1);
    }

    if ((err=AI_ContReadChannel(card, channel, AD_B_5_V, ai_buf, data_size, (F64)sample_rate, ASYNCH_OP)) != NoError) {
        printf("AI_ContReadPort Error = %d\n", err);
        Release_Card( card );
        free( ai_buf );
        exit(-1);
    }
    clear_op = FALSE;

    while (!fstop && !clear_op) {
        AI_AsyncCheck(card, &fstop, &count);
    }

    AI_AsyncClear(card, &count);
    Release_Card( card );

    if( write_to_file( ai_buf, channel+1 ) )
        printf("\n\nThe input data is already stored in file '%s'.\n", file_name);
    show_channel_data( ai_buf, channel+1 );
    free( ai_buf );

    printf("\n Press any key for exit\n" );
    getch();

    return 0;
}

BOOLEAN write_to_file( U16 *buf, int chan_idx )
{
    I16 adinput;
    U8 s[16];
    FILE *fp;
    U32 i;

    if (( fp = fopen( file_name , "w+b")) == NULL)
        return FALSE;

    fprintf( fp, " The data read from Channel#%d with Single Buffer Trigger mode :\n", chan_idx );
    for(i=0; i<count; i++){
        adinput = buf[i] >> 4;
        sprintf( s, "%04x", adinput );
        strcat(s, ((i+1)%12) ? "  ":"\n" );
        fwrite(s, sizeof(U8), strlen(s), fp);
    }

    fclose( fp );
    return TRUE;
}

void show_channel_data( U16 *buf, int chan_idx )
{
    U16 adinput;
    int channel_no, k;
    U32 i;

    printf(" >>>>>>>>>>>>>>> the first 240 data from Channel#%d  <<<<<<<<<<<<<<< \n", chan_idx );
    chan_idx --;
    for( i = 0, k=0; i< count && k< 240 ; i++ ){
        adinput = buf[i];
        channel_no = adinput & 0x000f;
        adinput = adinput >>4;

        if( channel_no == chan_idx ){
            printf("%04x  ", adinput );
            if( (k+1)%12 == 0 ) putchar('\n');
                k++;
        }
    }
}
