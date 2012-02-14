#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include "dask.h"

#define ChNum 2 

FILE *fout;

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 acq_ch[ChNum];
    U16 acq_gain[ChNum];
    U16 acq_value[ChNum];
    F64 voltage=0.0;
    int i;
    int j=0;

    for(i=0; i< ChNum; i++){
        acq_ch[i]=i;
        acq_gain[i]=AD_B_5_V;
    }  
    fout = fopen("acq.txt", "w");
    
    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if((card=Register_Card (PCI_9221, card_num)) < 0) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

    do{
        clrscr();
        err = AI_ReadMultiChannels(card, ChNum, acq_ch, acq_gain, acq_value);
        if(err < 0){
            printf("AI Read Multi Channel Error: %d\n", err);  
           exit(1);
        }
	for(i=0; i<ChNum; i++){
	    AI_VoltScale(card, acq_gain[i], acq_value[i], &voltage);
            fprintf(fout, "%f\t", voltage);
	}
        fprintf(fout, "\n");
        printf("Loop %d\n", j++);
    }while(!kb_hit());
    
    err = Release_Card(card);
    if(err < 0){
        printf("Release_Card error=%d", err);
        exit(1);
    }

    return 0;
}
