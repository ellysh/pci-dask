#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include "dask.h"

#define Channel 15
#define ReadCount (Channel+1)*1000
#define SampIntrv 16000
#define ScanIntrv SampIntrv*(Channel+1)


int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    //U16 ConfigCtrl = P9221_AI_Differential|P9221_AI_ExtTimeBase|P9221_TimeBaseSRC_GPI1;
    //U16 TrigCtrl = P9221_TRGMOD_ExtD|P9221_TRGSRC_GPI2;
    U16 ConfigCtrl = P9221_AI_SingEnded;
    U16 TrigCtrl = P9221_TRGMOD_SOFT;
    BOOLEAN AutoResetBuf = TRUE;
    U16 Buffer[ReadCount];
    U16 BufferId;
    FILE* fout;
    int i, j;
    BOOLEAN Stopped = FALSE;
    U32 AccessCnt = 0;
    F64 voltage;
    U16 AdRange = AD_B_5_V;
    
    if(!(fout=fopen("acq_data","w"))){
        printf("fopen Failed\n");
    }
    memset(Buffer, '\0', ReadCount*2);
    
    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if((card=Register_Card(PCI_9221, card_num)) < 0) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

    err = AI_SetTimeOut(card, 1000);
    if(err < 0){
        printf("AI_SetTimeOut error: %d\n", err);
        Release_Card(card);
	exit(1);
    }

    err = AI_9221_CounterInterval(card, ScanIntrv, SampIntrv);
    if(err < 0){
        printf("AI_9221_CounterInterval error: %d\n", err); 
        Release_Card(card);
	exit(1);
    }

    err = AI_9221_Config(card, ConfigCtrl, TrigCtrl, AutoResetBuf);
    if(err < 0){
        printf("AI_9221_Config error: %d\n", err);
        Release_Card(card);
	exit(1);
    }
   
    //printf("[APP] Virtual Address of Setup Buffer :0x%x\n", Buffer);
    err = AI_ContBufferSetup(card, Buffer, ReadCount, &BufferId);
    if(err < 0){
        printf("AI_ContBufferSetup error: %d\n", err);
        Release_Card(card);
	exit(1);
    }

    err = AI_ContScanChannels(card, Channel, AdRange, &BufferId, ReadCount, 0.0, ASYNCH_OP);
    if(err < 0){
        printf("AI cont Scan Read Channels error\n");
        Release_Card(card);
	exit(1);
    }
   
    i=0;
    do{
        err = AI_AsyncCheck(card, &Stopped, &AccessCnt);
        if(err<0){
	    printf("AI_AsyncCheck Error: %d\n", err);
	    break;
	}
	printf("loop%d: AccessCnt: %d\n", i, AccessCnt);
	i++;
    }while((!Stopped)&&(!kb_hit()));
    printf("i: %d, Stop: %d, AccessCnt: %d\n", i, Stopped, AccessCnt);
    
    err = AI_AsyncClear(card, &AccessCnt);
    if(err<0){
        printf("AI_AsyncClear Error: %d\n", err);
        Release_Card(card);
	exit(1);
    }
    printf("AccessCnt: %d\n", AccessCnt);
    
    if(!AutoResetBuf){
        err = AI_ContBufferReset(card);
        if(err < 0){
            printf("AI_ContBufferReset error: %d\n", err);
            Release_Card(card);
	    exit(1);
        }
    }

    for(i=0; i<1000; i++){
        for(j=0; j<(Channel+1); j++){
            err = AI_VoltScale(card, AdRange, Buffer[i*(Channel+1)+j], &voltage);
	    if(err < 0)
	        printf("AI_VoltScale Failed\n");
	    fprintf(fout,"%4.2f, ", (float)voltage);
	}
	fprintf(fout, "\n");
    }

    fclose(fout);
    Release_Card(card);
    printf("Press any key to exit...\n");
    getch();
    
    return 0;
}
