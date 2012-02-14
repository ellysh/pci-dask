#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "dask.h"
#include "conio.h"

int main(void)
{
    I16 err, card;
    U16 channel=3;
    U16 range=AD_B_10_V;
    U32 read_count=2048;
    F64 sample_rate=2048;
    U32 *ai_buf;
    int i;

    if((card=Register_Card(PCI_9114DG, 0))<0){
        printf("Register_Card error=%d", card);
        Release_Card(card);
        exit(1);
    }

    do{

        err = AI_9114_Config(card, TRIG_INT_PACER);
        if(err!=0){
            printf("AI_9114_Config error=%d", err);
            Release_Card(card);
            exit(1);
        }
        err = AI_AsyncDblBufferMode(card, 0);
        if(err!=0){
           printf("AI_DblBufferMode error=%d", err);
           Release_Card(card);
           exit(1);
        }

        ai_buf = (U32*)malloc(read_count*4);
        memset(ai_buf, '\0', read_count*4);

        err = AI_ContReadChannel(card, channel, range, ai_buf, read_count, sample_rate, SYNCH_OP);
        if(err!=0){
            printf("AI_ContReadChannel error=%d", err);
            Release_Card(card);
            free(ai_buf);
            exit(1);
        }
        free(ai_buf);
        printf("Loop %d\n", ++i);
    
    }while(!kb_hit());    

    Release_Card(card);
    return 0;
}

