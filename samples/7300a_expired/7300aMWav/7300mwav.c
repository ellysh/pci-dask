#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include "dask.h"
#include "conio.h"

#define MAX_SIZE    1024000   //in words 

U32 sample_rate=5000000;
U32 data_size=1000;
//U32 *pMem;
FILE *fp;
I16 card=-1, card_number = 0;
char *file_name="7300d.dat";
U16 DoBuf[102400];
BOOLEAN clear_op;
U16 in_buf[MAX_SIZE], in_buf2[MAX_SIZE], in_buf3[MAX_SIZE], in_buf4[MAX_SIZE]; 

BOOLEAN write_to_file( U16 *buf);

void * ProcessThread( void * temp_pt )
{
  BOOLEAN HalfReady;
  U32 count=0, count1;
  U16 j, viewidx[4];

//  data_size /= 2;
  printf("Press any key to stop...\n");
  if (( fp = fopen( file_name , "w+b")) == NULL)
	 	return FALSE;
  fprintf( fp, "ContReadPort with ASYNCH_OP mode \n");
  fprintf( fp, "count_per_page:%li\n", data_size );
  fprintf( fp, "channel_num:%d\n", 1 );
  fprintf( fp, "sample_rate(Hz):%li\n", sample_rate );
  fprintf( fp, "  DIGITAL INPUT   \n======================\n");
  j=0;
  do {
      //pMem[0] = 2048;
      HalfReady=0;
      do {
           DI_AsyncMultiBufferNextReady(card, &HalfReady, &viewidx[j]);
      } while (!HalfReady);
       j++;
if(j==4) break;
  } while (!clear_op);

  DI_AsyncClear(card, &count);
  printf("%d, %d, %d, %d\n",viewidx[0], viewidx[1], viewidx[2], viewidx[3]);
  printf("Starting to save data...\n");
  j=0;
  for(j=0; j<4; j++) {
		  switch(j) {
		  case 0:
				   write_to_file( in_buf );
				   break;
		  case 1:
				   write_to_file( in_buf2 );
				   break;
		  case 2:
				   write_to_file( in_buf3 );
				   break;
		  case 3:
				   write_to_file( in_buf4 );
				   break;
		  }
  }

  fclose( fp );
  printf("\n\ninput data are stored in file '%s'.\n", file_name);

  DO_AsyncClear(card, &count1);

  return NULL;
}



int main(void)
{
    I16 err;
    U32 i;
    pthread_t thread1;
    void * retval;
    U16 BufferId;

    setbuf( stdout, NULL );
    clrscr();
    printf(" ========================================================================\n");
    printf(" ==           7300 SAMPLE : PCI-7300 Multiple Buffered DMA             ==\n");
    printf(" ========================================================================\n\n\n");
    printf(" The output data is a sine wave and store data to file '%s'. It will not stop until you press a key.\n\nPress any key to start the operation.\n", file_name );

    getch();


  if ((card=Register_Card(PCI_7300A_RevA, card_number)) < 0) {
        printf("Register_Card error=%d\n", card);
        exit(1);
  }

  for (i=0; i<102400; i++)
       DoBuf[i] = (U16)/*(sin((double)i*M_PI/512)*0x7fffffff)+0x80000000*/(i%65536);

  for (i=0; i<MAX_SIZE; i++) {
       in_buf[i] = (U16)(0);
       in_buf2[i] = (U16)(0);
       in_buf3[i] = (U16)(0);
       in_buf4[i] = (U16)(0);

  }

//  pMem = (U32 *)malloc( data_size *sizeof(U32) );

         err=DI_7300A_Config(card, 16, TRIG_CLK_10MHZ, P7300_WAIT_NO, P7300_TERM_ON, 1, 1, 1);
         if (err !=0) {
               printf("DI_7300A_Config error=%d\n", err);
               Release_Card( card );
               exit(1);
        } 
        err=DO_7300A_Config (card, 16, TRIG_INT_PACER, P7300_WAIT_NO, P7300_TERM_ON, 1);
        if (err !=0) { 
               printf("DO_7300A_Config error=%d\n", err);
	       Release_Card( card );
     	       exit(1);
        } 


	err = DI_ContMultiBufferSetup (card, in_buf, data_size, &BufferId);
        if (err < 0) {
               printf("DI_ContMultiBufferSetup Error=%d\n", err);
               Release_Card( card );
               exit(1);
        }
	err = DI_ContMultiBufferSetup (card, in_buf2, data_size, &BufferId);
        if (err < 0) {
               printf("DI_ContMultiBufferSetup Error=%d\n", err);
               Release_Card( card );
               exit(1);
        }
	err = DI_ContMultiBufferSetup (card, in_buf3, data_size, &BufferId);
        if (err < 0) {
               printf("DI_ContMultiBufferSetup Error=%d\n", err);
               Release_Card( card );
               exit(1);
        }
 	err = DI_ContMultiBufferSetup (card, in_buf4, data_size, &BufferId);
        if (err < 0) {
               printf("DI_ContMultiBufferSetup Error=%d\n", err);
               Release_Card( card );
               exit(1);
        }
        clear_op = FALSE;
        if( pthread_create( &thread1, NULL, ProcessThread, NULL ) != 0 ){
               printf(" Can't create thread1 , terminate \n ");
               Release_Card(card);
               exit(-1);
        }
        if ((err=DO_ContWritePort(card, 0, DoBuf, 10240, 0, (F64)sample_rate, ASYNCH_OP)) != NoError) {
               printf("DO_ContWritePort error=%d", err);
               Release_Card( card );
               exit(1);
        }

        err = DI_ContMultiBufferStart (card, 0, 1);
        if (err < 0) {
               printf("DI_ContMultiBufferStart error=%d", err);
               DO_AsyncClear(card, &i);
               Release_Card( card );
               exit(1);
        }

       /*clear_op = FALSE;
       if( pthread_create( &thread1, NULL, ProcessThread, NULL ) != 0 ){
               printf(" Can't create thread1 , terminate \n ");
               Release_Card(card);
               exit(-1);
       }
       */
       getch();
       clear_op = TRUE;
       pthread_join(thread1, &retval);
       Release_Card(card);
       //free( pMem );
       exit(1);
}

BOOLEAN write_to_file( U16 *buf)
{
  U16 di_input;
  U8 s[100];
  U32 i;	

  for(i=0; i<data_size; i++){
    di_input = buf[i];
    s[0] = 0x00;
    sprintf( s, "%04x  \n", (U16)di_input );
    fwrite(s, sizeof(U8), strlen(s), fp);
  }

  return TRUE;
}



