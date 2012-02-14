#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dask.h"
#include "conio.h"

U16 channel=0;
U32 out_count=4096;
U32 samp_intrv = 50000;
U16 pattern1[4096];
U16 pattern2[4096];
U16 ao_buf[4096];
U16 ao_buf2[4096];

int main( void )
{
    I16 card, err, i, Id, j;
    int card_num;
    BOOLEAN halfReady;
    U32 count=0, count1;
    U16 viewidx;

    setbuf( stdout, NULL );
    clrscr();
    printf(" ========================================================================\n");
    printf(" ==     7300 SAMPLE : PCI-7300 Multiple Buffered DMA  (Output)         ==\n");
    printf(" ========================================================================\n\n\n");

    printf("Please input a card number: ");
    scanf(" %d", &card_num);
    // getch();

    if ((card=Register_Card (PCI_7300A_RevB, (I16)card_num)) <0 ) {
        printf("Register_Card error=%d\n", card);
        exit(1);
    }

    for (i=0;i<out_count;i++)
        pattern1[i] = (U16)(sin((double)i/512*M_PI)*0x7fff)+0x8000;
    for (i=0;i<out_count;i++)
        pattern2[i] = ((i*16)%65536) & 0xffff;

    memcpy(ao_buf, pattern1, out_count*2);
    memcpy(ao_buf2, pattern1, out_count*2);

    err=DO_7300B_Config (card, 16, TRIG_INT_PACER, P7300_WAIT_NO, P7300_TERM_ON, 0, 0x00040004);
    if (err !=0) {
        printf("DO_7300B_Config Error = %d, card=%d\n", err, card);
        Release_Card(card);
        exit(-1);
    }

    err = DO_ContMultiBufferSetup (card, ao_buf, out_count, &Id);
    if (err!=0) {
        printf("DO_ContMultiBufferSetup error=%d for the 1st buffer", err);
        Release_Card(card);
        exit(-2);
    }
    err = DO_ContMultiBufferSetup (card, ao_buf2, out_count, &Id);
    if (err!=0) {
        printf("DO_ContMultiBufferSetup error=%d for the 2nd buffer", err);
        Release_Card(card);
        exit(-3);
    }

    err = DO_ContMultiBufferStart (card, 0, samp_intrv);
    if (err!=0) {
        printf("DO_ContMultiBufferStart error=%d", err);
        Release_Card(card);
        exit(1);
    }

    j=0;
    printf("\n\nPress Enter to stop input operation.");
    printf("\n\nData count : \n");

    do {
        do {
            DO_AsyncMultiBufferNextReady(card, &halfReady, &viewidx);
        } while (!halfReady);
        if(halfReady) {
            if(!(j%4))
                memcpy(ao_buf, pattern2, out_count*2);
            else if((j%4)==1)
                memcpy(ao_buf2, pattern2, out_count*2);
            else if((j%4)==2)
                memcpy(ao_buf, pattern1, out_count*2);
            else
                memcpy(ao_buf2, pattern1, out_count*2);
            count += (out_count);
            printf("%d %ld\r", viewidx, count);
            j++;
        }
    } while(!kbhit());

    DO_AsyncClear(card, &count1);

    count += (count1);
    Release_Card(card);
    printf("\nPress ENTER to exit the program. "); getchar();

    return 0;
}
