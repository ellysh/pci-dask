#include <stdio.h>
#include <stdlib.h>
#include "dask.h"
#include "conio.h"

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 func_sel;
    
    U16 gptc_Ctr;
    U16 gptc_Mode;
    U16 gptc_SrcCtrl = 0;
    U16 gptc_PolCtrl = 0;
    U32 gptc_Reg1Val = 0;
    U32 gptc_Reg2Val = 0;
   
    U16 gptc_Status;
    U32 gptc_CtrVal;
    
    U8 conf_flag;
    U8 quit_flag;
    
    printf("Please input a card number: ");
    scanf(" %hd", &card_num);
    if((card=Register_Card (PCI_9221, card_num)) < 0) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

    printf("\n   (0) Edge Counting");
    printf("\n   (1) Pulse Measurement");
    printf("\n   (2) Pulse Generatation");
    printf("\n   (3) Encoder Mode");
    printf("\n   Please Select GPTC Mode: ");
    scanf(" %hd", &func_sel);
    printf("\n");
    switch(func_sel){
        case 0:
            printf("Edge Counting\n");
	        printf("Counter will up-count or down-count 1,\n");
            printf("while the state of Gate changes (0->1 or 1->0).\n");
	    
            printf("\nSelect General Purpose Timer/Counter - (0) or (1)? (Counter 0 is default): ");
            scanf(" %hd", &gptc_Ctr);
	        gptc_Ctr = (gptc_Ctr==1)? 1:0;
	    
	        printf("\n(E)xternal Gate or (I)nternal Gate? (Internal Clock is default): ");
            scanf(" %c", &conf_flag);
	        if((conf_flag=='e')||(conf_flag=='E'))
	            gptc_SrcCtrl |= GPTC_GATE_SRC_Ext;
	    
	        gptc_Mode = SimpleGatedEventCNT;
	    
	        GPTC_Clear(card, gptc_Ctr);
            GPTC_Setup(card, gptc_Ctr, gptc_Mode, gptc_SrcCtrl, gptc_PolCtrl, gptc_Reg1Val, gptc_Reg2Val);
	        GPTC_Control(card, gptc_Ctr, IntENABLE, 1);
	        printf("\n(U)p count or (D)own count? (down-count is default): ");
	        scanf(" %c", &conf_flag);
	        if((conf_flag=='u')||(conf_flag=='U'))
	            GPTC_Control(card, gptc_Ctr, IntUpDnCTR, 1);
	        else
		        GPTC_Control(card, gptc_Ctr, IntUpDnCTR, 0);

	        if(gptc_SrcCtrl&GPTC_GATE_SRC_Ext){
	            printf("\nExternal Gate");
		        do{
	        
	    
	            }while(!kb_hit());
	        }
	        else{
	            printf("\nInternal Gate");
                printf("\nPress any key to generate internal gate.\n");
		        getch();
	            do{
		            GPTC_Control(card, gptc_Ctr, IntGate, 1);
		            GPTC_Control(card, gptc_Ctr, IntGate, 0);
		     //       do{
		     //           GPTC_Status(card, gptc_Ctr, &gptc_Status);
		     //       }while(!(gptc_Status>>1));
		            GPTC_Read(card, gptc_Ctr, &gptc_CtrVal);
		            printf("\nCounter%d Value: %d", gptc_Ctr, (int)gptc_CtrVal);
                    printf("\nPress any key to generate internal gate or 'q' to quit.\n");
		            quit_flag = getch();
		       }while((quit_flag!='q')&&(quit_flag!='Q'));
	        }
            break;
	case 1:
	    printf("Pulse Measurement\n");
            break;
	case 2:
	    printf("Pulse Generation\n");
	    break;
	case 3:
	    printf("Encoder Mode\n");
            break;
	default:
            break;
    }
    
    err = Release_Card(card);
    if(err < 0){
        printf("Release_Card error=%d", err);
        exit(1);
    }
 
    return 0;
}

