#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include "dask.h"
#include "conio.h"

U32 sample_rate = 10000;
U32 data_size = 1024;
U32 *pMem;
FILE *fp;
I16 card = -1, card_number = 0;
char *file_name = "7200d.dat";
U32 DoBuf[10240];
BOOLEAN clear_op;

BOOLEAN write_to_file(U32 *buf);


void* ProcessThread(void *temp_pt)
{
    BOOLEAN HalfReady;
    U32 count = 0, count1;

    data_size /= 2;

    if((fp = fopen(file_name, "w+b"))==NULL)
        return FALSE;
    fprintf(fp, "ContReadPort with ASYNCH_OP mode \n");
    fprintf(fp, "count_per_page:%li\n", data_size);
    fprintf(fp, "channel_num:%d\n", 1);
    fprintf(fp, "sample_rate(Hz):%li\n", sample_rate);
    fprintf(fp, "  DIGITAL INPUT   \n======================\n");

    do{
        pMem[0] = 2048;
        do{
            DI_AsyncDblBufferHalfReady(card, &HalfReady);
        }while(!HalfReady);
        DI_AsyncDblBufferTransfer(card, pMem);
        write_to_file(pMem);
    }while(!clear_op);

    fclose(fp);
    printf("\n\ninput data are stored in file '%s'.\n", file_name);

    DI_AsyncClear(card, &count);
    DO_AsyncClear(card, &count1);

    return NULL;
}


int main(void)
{
    I16 err;
    U32 i;
    pthread_t thread1;
    void *retval;

    setbuf(stdout, NULL );
    clrscr();
    printf(" ========================================================================\n");
    printf(" ==             7200 SAMPLE : PCI-7200 Double Buffered DMA             ==\n");
    printf(" ========================================================================\n\n\n");
    printf(" The output data is a sine wave and store data to file '%s'. It will not stop until you press a key.\n\nPress any key to start the operation.\n", file_name );

    getch();

    if((card = Register_Card(PCI_7200, card_number))<0){
        printf("Register_Card error=%d\n", card);
        exit(1);
    }

    for(i=0; i<10240; i++)
        //DoBuf[i] = (U32)(sin((double)i*M_PI/512)*0x7fff)+0x8000;
        DoBuf[i] = (U32)(sin((double)i*M_PI/512)*0x7fffffff)+0x80000000;

    pMem = (U32 *)malloc(data_size *sizeof(U32));

    if((err = DO_7200_Config(card, TRIG_INT_PACER, OREQ_DISABLE, OTRIG_LOW))!=NoError){
        printf("DO_7200_Config error=%d\n", err);
        Release_Card(card);
        exit(1);
    }

    if((err = DI_7200_Config(card,TRIG_INT_PACER, DI_NOWAITING, DI_TRIG_FALLING, IREQ_FALLING))!=NoError){
        printf("DI_7200_Config error=%d\n", err);
        Release_Card(card);
        exit(1);
    }

    if((err = DO_AsyncDblBufferMode(card, 0))!=NoError){
        printf("DO_AsyncDblBufferMode error=%d\n", err);
        Release_Card(card);
        exit(1);
    }

    if((err = DI_AsyncDblBufferMode(card, 1))!=NoError){
        printf("DI_AsyncDblBufferMode error=%d\n", err);
        Release_Card(card);
        exit(1);
    }

    if((err = DO_ContWritePort(card, 0, DoBuf, 10240, 0, (F64)sample_rate, ASYNCH_OP))!=NoError){
        printf("DO_ContWritePort error=%d", err);
        Release_Card(card);
        exit(1);
    }

    if((err = DI_ContReadPort(card, 0, pMem, data_size, (F64)sample_rate, ASYNCH_OP))!=NoError){
        printf("DI_ContReadPort error=%d", err);
        DO_AsyncClear(card, &i);
        Release_Card(card);
        exit(1);
    }

    clear_op = FALSE;
    if(pthread_create(&thread1, NULL, ProcessThread, NULL)!=0){
        printf(" Can't create thread1 , terminate \n ");
        Release_Card(card);
        exit(-1);
    }

    getch();
    clear_op = TRUE;
    pthread_join(thread1, &retval);
    Release_Card(card);
    free(pMem);
    exit(1);
}


BOOLEAN write_to_file(U32 *buf)
{
    U32 di_input;
    U8 s[100];
    U32 i;

    for(i=0; i<data_size; i++){
        di_input = buf[i];
        s[0] = 0x00;
        sprintf(s, "%08lx  \n", (U32)di_input);
        fwrite(s, sizeof(U8), strlen(s), fp);
    }

    return TRUE;
}
