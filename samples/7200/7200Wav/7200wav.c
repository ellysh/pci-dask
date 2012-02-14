#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include "dask.h"
#include "conio.h"

I16 card = -1, card_number = 0;
U32 sample_rate = 100000;
U32 data_size = 102400;
U32 *pMem;
char *file_name = "7200d.dat";
U32 DoBuf[102400];
U32 out_count;

BOOLEAN write_to_file(U32 *buf);

int main(void)
{
    I16 card, err;
    BOOLEAN bStopped;
    U32 count,out_count, i;

    setbuf(stdout, NULL);
    clrscr();
    printf(" ========================================================================\n");
    printf(" ==                      7200 SAMPLE : PCI-7200 DMA                    ==\n");
    printf(" ========================================================================\n\n\n");
    printf("\nThe output data is a sine wave. The input data is stored to file '%s'.\n\nPress any key to start the operation.\n", file_name );

    getch();
    if((card = Register_Card(PCI_7200, card_number))<0){
        printf("Register_Card error=%d\n", card);
        exit(1);
    }

    for(i=0; i<10240; i++){
        if(i<10)
            DoBuf[i] = (U32)0;
        else
            DoBuf[i] = (U32)i;//((sin((double)i*M_PI/128)*0x7FFF)+0x8000);
           //DoBuf[i] = (U32)(sin((double)i*M_PI/128)*1024)+1024;
    }

    pMem = (U32 *)malloc(data_size*sizeof(U32));

    if((err = DO_7200_Config(card, TRIG_INT_PACER, OREQ_DISABLE, OTRIG_LOW))!=NoError){
      printf("DO_7200_Config error=%d\n", err);
      Release_Card(card);
      exit(1);
    }

    if((err = DI_7200_Config(card, TRIG_INT_PACER, DI_NOWAITING, DI_TRIG_FALLING, IREQ_FALLING))!=NoError){
        printf("D1_7200_Config error=%d\n", err);
        Release_Card(card);
        exit(1);
    }

    if((err = DO_AsyncDblBufferMode(card, 0))!=NoError){
        printf("DO_AsyncDblBufferMode error=%d", err);
        Release_Card(card);
        exit(1);
    }

    if((err = DI_AsyncDblBufferMode(card, 0))!=NoError){
        printf("DI_AsyncDblBufferMode error=%d", err);
        Release_Card(card);
        exit(1);
    }

    if((err = DO_ContWritePort(card, 0, DoBuf, 10240, 0, (F64)sample_rate, ASYNCH_OP))!=NoError){
        printf("DO_ContWritePort error=%d", err);
        Release_Card(card);
        exit(1);
    }

    if((err=DI_ContReadPort(card, 0, pMem, data_size, (F64)sample_rate, ASYNCH_OP))!=NoError){
        printf("DI_ContReadPort error=%d", err);
        Release_Card(card);
        exit(1);
    }

    do{
       DI_AsyncCheck(card, &bStopped, &count);
    }while(!bStopped);
    DI_AsyncClear(card, &count);
    DO_AsyncClear(card, &out_count);

    if(write_to_file(pMem))
        printf("\n\nThe input data is already stored in file '%s'.\n", file_name);

    if(card>=0)
        Release_Card(card);
    free(pMem);
    printf("\nPress ENTER to exit the program. "); getchar();
    return 0;
}


BOOLEAN write_to_file(U32 *buf)
{
    U8 s[100];
    FILE *fp;
    U32 i;

    if((fp = fopen(file_name , "w+b"))==NULL)
        return FALSE;
    fprintf(fp, "ContReadPort with ASYNCH_OP mode \n");
    fprintf(fp, "count_per_page:%li\n", data_size);
    fprintf(fp, "channel_num:%d\n", 1);
    fprintf(fp, "sample_rate(Hz):%li\n", (U32)sample_rate);
    fprintf(fp, "   DIGITAL INPUT DATA \n======================\n");

    for(i=0; i<data_size; i++){
        s[0] = 0x00;
        sprintf(s, "%08lx  \n", buf[i]&0xFFFFFFFF);
        fwrite(s, sizeof(U8), strlen(s), fp);
    }

    fclose(fp);
    return TRUE;
}
