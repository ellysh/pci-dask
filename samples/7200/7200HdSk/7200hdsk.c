#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dask.h"
#include "conio.h"

I16 card = -1, card_number = 0;
U32 sample_rate = 1000000;
U32 data_size = 4096;
U32 *pMem;
char *file_name = "7200d.dat";
U32 DoBuf[40970];

BOOLEAN write_to_file(U32 *buf);

int main(void)
{
    I16 err;
    BOOLEAN bStopped;
    U32 count, out_count, i;

    setbuf(stdout, NULL);
    clrscr();
    printf(" ========================================================================\n");
    printf(" ==                7200 SAMPLE : PCI-7200 HANDSHAKE DMA                ==\n");
    printf(" ========================================================================\n\n\n");
    printf("\nThe output data is a sine wave. The input data is stored to file '%s'.\n\nPress any key to start the operation.\n", file_name );

    getch();

    if((card = Register_Card(PCI_7200, card_number))<0){
        printf("Register_Card Error = %d\n", card);
        exit(0);
    }

    for(i=0; i<40970; i++)
        DoBuf[i] = (U32)((sin((double)i*M_PI/256)*0x7FFF)+0x8000);

    pMem = (U32 *)malloc(data_size*sizeof(U32));
    if(pMem==NULL){
        printf("No Memory\n");
        Release_Card(card);
        exit(0);
    }
    memset(pMem, '\0', data_size*sizeof(U32));

    if((err = DI_7200_Config(card, TRIG_HANDSHAKE, DI_NOWAITING, DI_TRIG_FALLING, IREQ_FALLING))!=NoError){
        printf("DI_Config Error = %d\n", err);
        Release_Card(card);
        exit(0);
    }
    if((err = DO_7200_Config(card, TRIG_HANDSHAKE, OREQ_ENABLE, OTRIG_LOW))!=NoError){
        printf("DO_Config Error = %d\n", err);
        Release_Card(card);
        exit(0);
    }
    if((err = DI_AsyncDblBufferMode(card, 0))!=NoError){
        printf("DI_AsyncDblBufferMode Error = %d\n", err);
        Release_Card(card);
        exit(0);
    }
    if((err = DO_AsyncDblBufferMode(card, 0))!=NoError){
        printf("DI_AsyncDblBufferMode Error = %d\n", err);
        Release_Card(card);
        exit(0);
    }
    if((err = DI_ContReadPort(card, 0, pMem, data_size, (F64)sample_rate, ASYNCH_OP))!=NoError){
        printf("DI_ContReadPort Error = %d\n", err);
        Release_Card(card);
        exit(0);
    }
    if((err = DO_ContWritePort(card, 0, DoBuf, data_size+2, 1, (F64)sample_rate, ASYNCH_OP))!=NoError){
        printf("DO_ContWritePort Error = %d\n", err);
        Release_Card(card);
        exit(0);
    }

    do{
        DI_AsyncCheck(card, &bStopped, &count);
    }while(!bStopped);

    DI_AsyncClear(card, &count);
    DO_AsyncClear(card, &out_count);

    if(write_to_file(pMem)){
        printf(" the digital data is stored in the file : %s , prees any key for exit\n", file_name);
        getch();
    }

    free(pMem);
    Release_Card(card);
    return 0;
}


BOOLEAN write_to_file(U32 *buf)
{
    U8 s[100];
    FILE *fp;
    U32 i;

    if((fp = fopen(file_name , "w+b"))==NULL)
        return FALSE;
    fprintf(fp, "ContReadPort with HANDSHAKE mode \n");
    fprintf(fp, "count_per_page:%li\n", data_size);
    fprintf(fp, "channel_num:%d\n", 1);
    fprintf(fp, "sample_rate(Hz):%li\n", (U32)sample_rate);
    fprintf(fp, "   DIGITAL INPUT DATA \n======================\n");

    for(i=0; i<data_size; i++){
        s[0] = 0x00;
        sprintf(s, "%08lx  \n", buf[i]&0x0000FFFF);
        fwrite(s, sizeof(U8), strlen(s), fp);
    }

    fclose(fp);
    return TRUE;
}
