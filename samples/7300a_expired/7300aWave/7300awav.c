#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dask.h"

#define MAX_SIZE    1024000   //in words

I16 card=-1, card_number = 0;
U16 in_buf[MAX_SIZE], out_buf[MAX_SIZE];
int oldinput=0,adinput;
U32 data_size=100000;
char *file_name="7300a.dat";

BOOLEAN write_to_file( U16 *buf);

int main(void)
{
    I16 card, err;
    BOOLEAN stopped;
    U32 access_cnt, i;

    setbuf( stdout, NULL );
    clrscr();
    printf(" ========================================================================\n");
    printf(" ==                  7300A SAMPLE : DIO simultaneously                 ==\n");
    printf(" ========================================================================\n\n\n");
    printf("This sample perform DMA DI in 10MHz and DMA DO in 5MHZ rate simultaneously.\nThe output data is a sine wave. The input data is stored to file '%s'.\n\nPress ENTER to start the operation.\n", file_name );

    getchar();

    if ((card=Register_Card (PCI_7300A_RevA, 0)) <0 ) {
        printf("Register_Card error=%d\n", card);
        exit(1);
    }

    err=DI_7300A_Config(card, 16, TRIG_CLK_10MHZ, P7300_WAIT_NO, P7300_TERM_ON, 1, 1, 1);
    if (err!=0) {
       printf("DI_7300B_Config error=%d\n", err);
       exit(1);
    }

    err=DO_7300A_Config (card, 16, TRIG_INT_PACER, P7300_WAIT_NO, P7300_TERM_ON, 1);
    if (err!=0) {
       printf("DO_7300B_Config error=%d\n", err);
       exit(1);
    }

    for (i=0; i<MAX_SIZE; i++)
           out_buf[i] = (U16)(sin((double)i/5000*M_PI)*0x7fff)+0x8000;
    for (i=0; i<MAX_SIZE; i++)
           in_buf[i] = (U16)(i%65536);

    err = DO_ContWritePort (card, 0, out_buf, data_size, 3, (F64)5000000, ASYNCH_OP);
    if (err!=0) {
       printf("DO_ContWritePort error=%d", err);
       Release_Card( card );
       exit(1);
    }

    err = DI_ContReadPort (card, 0, in_buf, data_size, (F64)1, ASYNCH_OP);
    if (err!=0) {
       printf("DI_ContReadPort error=%d", err);
       DO_AsyncClear(card, &access_cnt);
       Release_Card( card );
    }

    do {
      DO_AsyncCheck (card, &stopped, &access_cnt);
    } while (!stopped);
    DO_AsyncClear (card, &access_cnt);

    do {
      DI_AsyncCheck (card, &stopped, &access_cnt);
    } while (!stopped);
    DI_AsyncClear (card, &access_cnt);
	
    if( write_to_file( in_buf ) )
    printf("\n\nThe input data is already stored in file '%s'.\n", file_name);

    Release_Card(card);

    printf("\nPress ENTER to exit the program. "); getchar();

    return 0;
}

BOOLEAN write_to_file( U16 *buf)
{
  U8 s[100];
  FILE *fp;
  U32 i;
	
  if (( fp = fopen( file_name , "w+b")) == NULL)
	 	return FALSE;
  fprintf( fp, "ContReadPort with ASYNCH_OP mode \n");
  fprintf( fp, "count_per_page:%li\n", data_size );
  fprintf( fp, "channel_num:%d\n", 1 );
  fprintf( fp, "sample_rate(Hz):%li\n", (U32)10000000 );
  fprintf( fp, "CH0   CH1   CH2   CH3 \n======================\n");

  for(i=0; i<data_size; i++){
    s[0] = 0x00;
    sprintf( s, "%04x  \n", buf[i] );
    fwrite(s, sizeof(U8), strlen(s), fp);
  }

  fclose( fp );
  return TRUE;
}





