#include <stdio.h>
#include <stdlib.h>
#include "dask.h"


I16 card;

char *file_name="9112dat.txt";

long data_size=4000, mem_size = 0;
long sample_rate=10000;
U16 channel=0, range=AD_B_5_V;

U16 *ai_buf;

BOOLEAN write_to_file( U16 *buf, int num_chan );

int main(void)
{
    I16 err;

    if ((card = Register_Card (PCI_9112, 0)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }
    err = AI_9112_Config(card,TRIG_INT_PACER);
    if (err!=0) {
        printf("AI_9112_Config error=%d", err);
        Release_Card(card);
        exit(1);
    }
    err = AI_AsyncDblBufferMode(card, 0);
    if (err!=0) {
        printf("AI_DblBufferMode error=%d", err);
        Release_Card(card);
        exit(1);
    }
    mem_size=data_size*2;
    ai_buf = (U16*)malloc(mem_size);

    //AI_ContScanChannels (card, channel, range, ai_buf, data_size, (F64)sample_rate, SYNCH_OP);
    err = AI_ContReadChannel (card, channel, range, ai_buf, data_size, (F64)sample_rate, SYNCH_OP);

    if (err!=0) {
        printf("AI_ContReadChannel error=%d", err);
        Release_Card(card);
        free( ai_buf );
        exit(1);
    }

    Release_Card(card);
    if( write_to_file( ai_buf, channel+1 ) )
        printf("\n\nThe input data is already stored in file '%s'.\n", file_name);
    free( ai_buf );
    printf("\nPress ENTER to exit the program. "); getchar();

    exit(0);
}


BOOLEAN write_to_file( U16 *buf, int num_chan )
{
    // int chan_no;
    U8 temp_s[16];
    FILE *fp;
    U32 i;

    if (( fp = fopen( file_name , "w+b")) == NULL)
        return FALSE;

    fprintf( fp, " ContReadChannel with SYNCH_OP mode \n\n\n");

    for(i=0; i<data_size; i++){
        sprintf( temp_s, "%04x\n", buf[i] );
        fwrite(temp_s, sizeof(U8), strlen(temp_s), fp);
    }

    fclose( fp );
    return TRUE;
}
