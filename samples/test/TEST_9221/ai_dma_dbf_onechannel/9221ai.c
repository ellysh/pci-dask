#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include "dask.h"

#define BufSize 2500 

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    //U16 ConfigCtrl = P9221_AI_Differential|P9221_AI_ExtTimeBase|P9221_TimeBaseSRC_GPI1;
    //U16 TrigCtrl = P9221_TRGMOD_ExtD|P9221_TRGSRC_GPI2;
    U16 ConfigCtrl = P9221_AI_SingEnded;
    U16 TrigCtrl = P9221_TRGMOD_SOFT;
    BOOLEAN AutoResetBuf = TRUE;
    
    U32 ScanIntrv = 1600;
    U32 SampIntrv = 1600;
    
    U16 DataStore[BufSize*10];
    U16 Buffer1[BufSize];
    U16 Buffer2[BufSize];
    U16 BufferId1, BufferId2;

    U16 Channel = 0;
    U16 AdRange = AD_B_5_V;
    
    BOOLEAN HalfReady = FALSE;
    BOOLEAN StopFlag = FALSE;
    BOOLEAN overrunFlag = FALSE;
    U32 AccessCnt = 0;
    F64 voltage = 0.0;
    FILE* fout;
    int ViewIndex;
    int Loop;
    int i;
    
    if(!(fout=fopen("acq_data","w"))){
        printf("fopen Failed\n");
    }
    memset(DataStore, '\0', BufSize*10*2);
    memset(Buffer1, '\0', BufSize*2);
    memset(Buffer2, '\0', BufSize*2);
    
    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if((card=Register_Card(PCI_9221, card_num)) < 0){
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
   
    AI_AsyncDblBufferMode(card, 1);
    if(err < 0){
        print("AI_AsyncDblBufferMode error: %d\n", err);
	Release_Card(card);
	exit(1);
    }

    err = AI_ContBufferSetup(card, Buffer1, BufSize, &BufferId1);
    if(err < 0){
        printf("AI_ContBufferSetup1 error: %d\n", err);
        Release_Card(card);
	exit(1);
    }
    
    err = AI_ContBufferSetup(card, Buffer2, BufSize, &BufferId2);
    if(err < 0){
        printf("AI_ContBufferSetup2 error: %d\n", err);
        Release_Card(card);
	exit(1);
    }
  
    err = AI_ContReadChannel(card, Channel, AdRange, &BufferId1, 0, 0.0, ASYNCH_OP);
    if(err < 0){
        printf("AI count Read Channels error\n");
        Release_Card(card);
	exit(1);
    }
     
    ViewIndex=0;
    Loop=0;
    do{
        err = AI_AsyncDblBufferHalfReady(card, &HalfReady, &StopFlag);
	if(err<0){
	    printf("AI_AsyncDblBufferHalfReady error: %d\n", err);
	    break;
	}
	err = AI_AsyncDblBufferOverrun(card, 0, &overrunFlag);
	if(err<0)
	    printf("AI_AsyncDblBufferOverrun error: %d\n", err);
        if(overrunFlag)
	    printf("OVERRUN\n");
	
	if(HalfReady){
	    printf("Loop: %d, Buffer Half Ready: %d\n", Loop, ViewIndex);
	    if(ViewIndex){
	        memcpy(DataStore+Loop*BufSize, Buffer2, BufSize*2);
		AI_AsyncDblBufferHandled(card);
	        memset(Buffer2, '\0', BufSize*2);
	    }
	    else{
	        memcpy(DataStore+Loop*BufSize, Buffer1, BufSize*2);
	        AI_AsyncDblBufferHandled(card);
		memset(Buffer1, '\0', BufSize*2);
	    }
	    ViewIndex = (++ViewIndex)%2;
	    Loop = (++Loop)%10;
	}
    }while((!StopFlag)&&(!kb_hit()));
    
    err = AI_AsyncClear(card, &AccessCnt);
    if(err<0){
        printf("AI_AsyncClear Error: %d\n", err);
        Release_Card(card);
	exit(1);
    }
    printf("AccessCnt: %d\n", AccessCnt);
    if(ViewIndex){
        printf("Loop: %d, ViewIndex: %d\n", Loop, ViewIndex);
	memcpy(DataStore+Loop*BufSize, Buffer2, /*BufSize*/AccessCnt*2);
    }
    else{
        printf("Loop: %d, ViewIndex: %d\n", Loop, ViewIndex);
        memcpy(DataStore+Loop*BufSize, Buffer1, /*BufSize*/AccessCnt*2);
    }
    printf("End Point: %d\n", Loop*BufSize+AccessCnt); 
    if(!AutoResetBuf){
        err = AI_ContBufferReset(card);
        if(err < 0){
            printf("AI_ContBufferReset error: %d\n", err);
            Release_Card(card);
	    exit(1);
        }
    }

    for(i=0; i<BufSize*10; i++){
        err = AI_VoltScale(card, AdRange, DataStore[i], &voltage);
	if(err < 0)
	    printf("AI_VoltScale Failed\n");
	fprintf(fout,"%f\n", (float)voltage);
    }

    fclose(fout);
    Release_Card(card);
    printf("Press any key to exit...\n");
    getch();
    
    return 0;
}
