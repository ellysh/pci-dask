#include <stdio.h>
#include <stdlib.h>
#include "dask.h"
#include "conio.h"

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    
    U16 GCtr = 0;
    U16 Mode = 0;
	U16 SrcCtrl = 0;
	U16 PolCtrl = 0;
	U32 Reg1_Val = 0;
	U32 Reg2_Val = 0;
    U32 Value;
    U16 Stat;
    int i;
    char qflag;

    printf("Please input a card number: ");
    scanf(" %hd", &card_num);
    if((card=Register_Card (PCI_9221, card_num)) < 0) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

/*Mode 1*/
	/*------------------------
	/ Edge Count - Internal
	/------------------------*/
/*	Mode = SimpleGatedEventCNT;
	Reg1_Val = 10;
	Reg2_Val = 20;
	GPTC_Clear(card, GCtr);
	GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, Reg1_Val, Reg2_Val);
	GPTC_Control(card, GCtr, IntENABLE, 1);
    GPTC_Control(card, GCtr, IntUpDnCTR, 0);
	GPTC_Control(card, GCtr, IntGate, 0);
    for(i=0; i<10; i++){
		GPTC_Control(card, GCtr, IntGate, 1);
		GPTC_Control(card, GCtr, IntGate, 0);
		GPTC_Read(card, GCtr, &Value);
		printf("GPTC%d Value: %d\n", GCtr, Value);
    }
	GPTC_Clear(card, GCtr);

	/*------------------------
	/ Edge Count - External
	/------------------------*/
/*	Mode = SimpleGatedEventCNT;
    SrcCtrl = GPTC_GATE_SRC_Ext|GPTC_UPDOWN_Ext;
    PolCtrl = GPTC_GATE_LACTIVE|GPTC_UPDOWN_LACTIVE;
	Reg1_Val = 10;
	err = GPTC_Clear(card, GCtr);
	err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, Reg1_Val, Reg2_Val);
    err = DO_WritePort(card, 0, 0x4);
	err = GPTC_Control(card, GCtr, IntENABLE, 1);
    for(i=0; i<10; i++){
		err = DO_WritePort(card, 0, 0x0);
		err = DO_WritePort(card, 0, 0x4);
		err = GPTC_Read(card, GCtr, &Value);
		printf("GPTC%d Value: %d\n", GCtr, Value);
    }
	err = GPTC_Clear(card, GCtr);

/*Mode 2*/
	/*------------------------
	/ Period MSR
	/------------------------*/
/*	Mode = SinglePeriodMSR;
	SrcCtrl = GPTC_GATE_SRC_Ext;
	err = GPTC_Clear(card, GCtr);
	err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, Reg1_Val, Reg2_Val);
	err = GPTC_Control(card, GCtr, IntUpDnCTR, 1);
	err = GPTC_Control(card, GCtr, IntENABLE, 1);
	do{
		err = GPTC_Status(card, GCtr, &Stat);
        usleep(100000);
    }while(!((Stat>>1)&0x1));
	err = GPTC_Read(card, GCtr, &Value);
	printf("GPTC%d Value: 0x%x\n", GCtr, Value);
    printf("Period %f s\n", ((float)Value)/40000000);
	err = GPTC_Clear(card, GCtr);

/*Mode 3*/
	/*------------------------
	/ Period Width
	/------------------------*/
/*	Mode = SinglePulseWidthMSR;
	SrcCtrl = GPTC_GATE_SRC_Ext;
	err = GPTC_Clear(card, GCtr);
	err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, Reg1_Val, Reg2_Val);
	err = GPTC_Control(card, GCtr, IntUpDnCTR, 1);
	err = GPTC_Control(card, GCtr, IntENABLE, 1);
	do{
		err = GPTC_Status(card, GCtr, &Stat);
        usleep(100000);
    }while(!((Stat>>1)&0x1));
	err = GPTC_Read(card, GCtr, &Value);
	printf("GPTC%d Value: 0x%x\n", GCtr, Value);
    printf("Width %f s\n", ((float)Value)/40000000);
	err = GPTC_Clear(card, GCtr);

/*Mode 4*/
	/*---------------------------------------------
	/ Single Gated Pulse Generation, Software Gate
	/---------------------------------------------*/
/*	Mode = SingleGatedPulseGen;
	Reg1_Val = 8000000;
	err = GPTC_Clear(card, GCtr);
	err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, Reg1_Val, Reg2_Val);
    printf("press any key to enable counter...\n");
	getch();
	err = GPTC_Control(card, GCtr, IntENABLE, 1);
    printf("press any key to generate pulse...\n");
	getch();
    err = GPTC_Control(card, GCtr, IntGate, 1);
    printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("press any key to exit...\n");
    getch();
	err = GPTC_Clear(card, GCtr);

	/*---------------------------------------------
	/ Single Gated Pulse Generation, External Gate
	/---------------------------------------------*/
/*	Mode = SingleGatedPulseGen;
	SrcCtrl = GPTC_GATE_SRC_Ext;
	Reg1_Val = 8000000;
	err = GPTC_Clear(card, GCtr);
	err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, Reg1_Val, Reg2_Val);
    printf("press any key to enable counter...\n");
	getch();
	err = GPTC_Control(card, GCtr, IntENABLE, 1);
    printf("press any key to generate pulse...\n");
	getch();
	err = DO_WriteLine(card, 0, 2, 1);
    printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("press any key to exit...\n");
    getch();
	err = GPTC_Clear(card, GCtr);

/*Mode 5*/
	/*---------------------------------------------
	/ Single Triggered Pulse Generation, Software Gate
	/---------------------------------------------*/
/*	Mode = SingleTrigPulseGen;
	Reg1_Val = 8000000;
	err = GPTC_Clear(card, GCtr);
	err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, Reg1_Val, Reg2_Val);
    printf("press any key to enable counter...\n");
	getch();
	err = GPTC_Control(card, GCtr, IntENABLE, 1);
    printf("press any key to generate pulse...\n");
	getch();
	err = GPTC_Control(card, GCtr, IntGate, 0);
    err = GPTC_Control(card, GCtr, IntGate, 1);
	err = GPTC_Control(card, GCtr, IntGate, 0);
    printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("press any key to exit...\n");
    getch();
	err = GPTC_Clear(card, GCtr);

	/*---------------------------------------------
	/ Single Triggered Pulse Generation, External Gate
	/---------------------------------------------*/
/*	Mode = SingleTrigPulseGen;
	SrcCtrl = GPTC_GATE_SRC_Ext;
	Reg1_Val = 8000000;
	err = GPTC_Clear(card, GCtr);
	err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, Reg1_Val, Reg2_Val);
    printf("press any key to enable counter...\n");
	getch();
	err = GPTC_Control(card, GCtr, IntENABLE, 1);
    printf("press any key to generate pulse...\n");
	getch();
	err = DO_WriteLine(card, 0, 2, 0);
	err = DO_WriteLine(card, 0, 2, 1);
	err = DO_WriteLine(card, 0, 2, 0);
    printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("press any key to exit...\n");
    getch();
	err = GPTC_Clear(card, GCtr);

/*Mode 6*/
	/*---------------------------------------------
	/ Re-Trigger Single Pulse Generation, Software Gate
	/---------------------------------------------*/
/*	Mode = RetrigSinglePulseGen;
	Reg1_Val = 8000000;
	err = GPTC_Clear(card, GCtr);
	err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, Reg1_Val, Reg2_Val);
    printf("press any key to enable counter...\n");
	getch();
	err = GPTC_Control(card, GCtr, IntENABLE, 1);
    printf("press any key to generate pulse...\n");
	getch();
    do{
	    err = GPTC_Control(card, GCtr, IntGate, 0);
    	err = GPTC_Control(card, GCtr, IntGate, 1);
		err = GPTC_Control(card, GCtr, IntGate, 0);
    	qflag = getch();
	}while(qflag!='q');
	printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("press any key to exit...\n");
    getch();
	err = GPTC_Clear(card, GCtr);

	/*---------------------------------------------
	/ Re-Trigger Single Pulse Generation, External Gate
	/---------------------------------------------*/
/*	Mode = RetrigSinglePulseGen;
	SrcCtrl = GPTC_GATE_SRC_Ext;
	Reg1_Val = 8000000;
	err = GPTC_Clear(card, GCtr);
	err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, Reg1_Val, Reg2_Val);
    printf("press any key to enable counter...\n");
	getch();
	err = GPTC_Control(card, GCtr, IntENABLE, 1);
    printf("press any key to generate pulse...\n");
	getch();
	do{
	    err = DO_WriteLine(card, 0, 2, 0);
		err = DO_WriteLine(card, 0, 2, 1);
		err = DO_WriteLine(card, 0, 2, 0);
		qflag = getch();
	}while(qflag!='q');
    printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("press any key to exit...\n");
    getch();
	err = GPTC_Clear(card, GCtr);

/*Mode 7*/
	/*---------------------------------------------
	/ Single Triggered Cont. Pulse Generation, Software Gate
	/---------------------------------------------*/
/*	Mode = SingleTrigContPulseGen;
	Reg1_Val = 8000000;
	err = GPTC_Clear(card, GCtr);
	err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, Reg1_Val, Reg2_Val);
    printf("press any key to enable counter...\n");
	getch();
	err = GPTC_Control(card, GCtr, IntENABLE, 1);
    printf("press any key to generate pulse...\n");
	getch();
	err = GPTC_Control(card, GCtr, IntGate, 0);
    err = GPTC_Control(card, GCtr, IntGate, 1);
	err = GPTC_Control(card, GCtr, IntGate, 0);
    printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("press any key to exit...\n");
    getch();
	err = GPTC_Clear(card, GCtr);

	/*---------------------------------------------
	/ Single Triggered Cont. Pulse Generation, External Gate
	/---------------------------------------------*/
/*	Mode = SingleTrigContPulseGen;
	SrcCtrl = GPTC_GATE_SRC_Ext;
	Reg1_Val = 8000000;
	err = GPTC_Clear(card, GCtr);
	err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, Reg1_Val, Reg2_Val);
    printf("press any key to enable counter...\n");
	getch();
	err = GPTC_Control(card, GCtr, IntENABLE, 1);
    printf("press any key to generate pulse...\n");
	getch();
	err = DO_WriteLine(card, 0, 2, 0);
	err = DO_WriteLine(card, 0, 2, 1);
	err = DO_WriteLine(card, 0, 2, 0);
    printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("press any key to exit...\n");
    getch();
	err = GPTC_Clear(card, GCtr);

/*Mode 8*/
	/*---------------------------------------------
	/ Single Gated Cont. Pulse Generation, Software Gate
	/---------------------------------------------*/
/*	Mode = ContGatedPulseGen;
	Reg1_Val = 8000000;
	err = GPTC_Clear(card, GCtr);
	err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, Reg1_Val, Reg2_Val);
    printf("press any key to enable counter...\n");
	getch();
	err = GPTC_Control(card, GCtr, IntENABLE, 1);
    printf("press any key to generate pulse...\n");
	getch();
    err = GPTC_Control(card, GCtr, IntGate, 1);
    printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("press any key to exit...\n");
    getch();
	err = GPTC_Clear(card, GCtr);

	/*---------------------------------------------
	/ Single Gated Cont. Pulse Generation, External Gate
	/---------------------------------------------*/
/*	Mode = ContGatedPulseGen;
	SrcCtrl = GPTC_GATE_SRC_Ext;
	Reg1_Val = 8000000;
	err = GPTC_Clear(card, GCtr);
	err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, Reg1_Val, Reg2_Val);
    printf("press any key to enable counter...\n");
	getch();
	err = GPTC_Control(card, GCtr, IntENABLE, 1);
    printf("press any key to generate pulse...\n");
	getch();
	err = DO_WriteLine(card, 0, 2, 1);
    printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("press any key to exit...\n");
    getch();
	err = GPTC_Clear(card, GCtr);

/*Mode 10*/
	/*---------------------------------------------
	/ Single Triggered Cont. PWM Pulse Generation, Software Gate
	/---------------------------------------------*/
/*	Mode = SingleTrigContPulseGenPWM;
	Reg1_Val = 8000000;
	Reg2_Val = 4000000;
	err = GPTC_Clear(card, GCtr);
	err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, Reg1_Val, Reg2_Val);
    printf("press any key to enable counter...\n");
	getch();
	err = GPTC_Control(card, GCtr, IntENABLE, 1);
    printf("press any key to generate pulse...\n");
	getch();
	err = GPTC_Control(card, GCtr, IntGate, 0);
    err = GPTC_Control(card, GCtr, IntGate, 1);
	err = GPTC_Control(card, GCtr, IntGate, 0);
    printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("Low width: %f s\n", ((float)Reg2_Val)/40000000);
	printf("press any key to exit...\n");
    getch();
	err = GPTC_Clear(card, GCtr);

	/*---------------------------------------------
	/ Single Triggered Cont. PWM Pulse Generation, External Gate
	/---------------------------------------------*/
/*	Mode = SingleTrigContPulseGenPWM;
	SrcCtrl = GPTC_GATE_SRC_Ext;
	Reg1_Val = 8000000;
	Reg2_Val = 4000000;
	err = GPTC_Clear(card, GCtr);
	err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, Reg1_Val, Reg2_Val);
    printf("press any key to enable counter...\n");
	getch();
	err = GPTC_Control(card, GCtr, IntENABLE, 1);
    printf("press any key to generate pulse...\n");
	getch();
	err = DO_WriteLine(card, 0, 2, 0);
	err = DO_WriteLine(card, 0, 2, 1);
	err = DO_WriteLine(card, 0, 2, 0);
    printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("Low width: %f s\n", ((float)Reg2_Val)/40000000);
	printf("press any key to exit...\n");
    getch();
	err = GPTC_Clear(card, GCtr);

/*Mode 11*/
	/*---------------------------------------------
	/ Single Gated Cont. PWM Pulse Generation, Software Gate
	/---------------------------------------------*/
/*	Mode = ContGatedPulseGenPWM;
	Reg1_Val = 8000000;
	Reg2_Val = 4000000;
	err = GPTC_Clear(card, GCtr);
	err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, Reg1_Val, Reg2_Val);
    printf("press any key to enable counter...\n");
	getch();
	err = GPTC_Control(card, GCtr, IntENABLE, 1);
    printf("press any key to generate pulse...\n");
	getch();
    err = GPTC_Control(card, GCtr, IntGate, 1);
    printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("Low width: %f s\n", ((float)Reg2_Val)/40000000);
	printf("press any key to exit...\n");
    getch();
	err = GPTC_Clear(card, GCtr);

	/*---------------------------------------------
	/ Single Gated Cont. Pulse Generation, External Gate
	/---------------------------------------------*/
/*	Mode = ContGatedPulseGenPWM;
	SrcCtrl = GPTC_GATE_SRC_Ext;
	Reg1_Val = 8000000;
	Reg2_Val = 4000000;
	err = GPTC_Clear(card, GCtr);
	err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, Reg1_Val, Reg2_Val);
    printf("press any key to enable counter...\n");
	getch();
	err = GPTC_Control(card, GCtr, IntENABLE, 1);
    printf("press any key to generate pulse...\n");
	getch();
	err = DO_WriteLine(card, 0, 2, 1);
    printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("Low width: %f s\n", ((float)Reg2_Val)/40000000);
	printf("press any key to exit...\n");
    getch();
	err = GPTC_Clear(card, GCtr);

/*Mode9*/
	/*---------------------------------------------
	/ Edge Separation MSR
	/---------------------------------------------*/
	// 6 and 21
/*	GPTC_Clear(card, 0);
	GPTC_Clear(card, 1);
	err = GPTC_Setup(card, 0, EdgeSeparationMSR, GPTC_GATE_SRC_Ext, 0, 0, 0);
	err = GPTC_Setup(card, 1, ContGatedPulseGen, GPTC_GATE_SRC_Ext, 0, 8000000, 0);
	DO_WriteLine(card, 0, 2, 0);
	printf("press any key to enable counter\n");
	getch();
	err = GPTC_Control(card, 0, IntENABLE, 1);
	err = GPTC_Control(card, 1, IntENABLE, 1);
	err = GPTC_Control(card, 0, IntUpDnCTR, 1);
	err = GPTC_Control(card, 1, IntUpDnCTR, 1);
	printf("press any key to generate pulse...\n");
	getch();
	err = DO_WriteLine(card, 0, 2, 1);
	do{
		err = GPTC_Status(card, 0, &Stat);
    	usleep(100000);
	}while(!((Stat>>1)&0x1));
	err = GPTC_Read(card, 0, &Value);
	printf("GPTC%d Value: 0x%x\n", GCtr, Value);
	printf("Period %f s\n", ((float)Value)/40000000);
	printf("press any key to exit...\n");
	getch();
	err = GPTC_Clear(card, 0);
	err = GPTC_Clear(card, 1);

	/*---------------------------------------------
	/ Edge Separation MSR
	/---------------------------------------------*/
	// 3 and 22
/*	GPTC_Clear(card, 1);
	GPTC_Clear(card, 0);
	err = GPTC_Setup(card, 1, EdgeSeparationMSR, GPTC_GATE_SRC_Ext, 0, 0, 0);
	err = GPTC_Setup(card, 0, ContGatedPulseGen, GPTC_GATE_SRC_Ext, 0, 8000000, 0);
	DO_WriteLine(card, 0, 2, 0);
	printf("press any key to enable counter\n");
	getch();
	err = GPTC_Control(card, 1, IntENABLE, 1);
	err = GPTC_Control(card, 0, IntENABLE, 1);
	err = GPTC_Control(card, 1, IntUpDnCTR, 1);
	err = GPTC_Control(card, 0, IntUpDnCTR, 1);
	printf("press any key to generate pulse...\n");
	getch();
	err = DO_WriteLine(card, 0, 2, 1);
	do{
		err = GPTC_Status(card, 1, &Stat);
    	usleep(100000);
	}while(!((Stat>>1)&0x1));
	err = GPTC_Read(card, 1, &Value);
	printf("GPTC%d Value: 0x%x\n", GCtr, Value);
	printf("Period %f s\n", ((float)Value)/40000000);
	printf("press any key to exit...\n");
	getch();
	err = GPTC_Clear(card, 1);
	err = GPTC_Clear(card, 0);

/*Mode Enconder*/
	Mode = CW_CCW_Encoder;/*x1_AB_Phase_Encoder;*//*x2_AB_Phase_Encoder;*//*x4_AB_Phase_Encoder;*/
	err = GPTC_Clear(card, GCtr);
	err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, Reg1_Val, Reg2_Val);
	//err = GPTC_Setup(card, GCtr, Phase_Z, SrcCtrl, PolCtrl, GPTC_EZ0_ClearPhase0, GPTC_EZ0_ClearMode0);
    printf("press any key to start...\n");
	getch();
	err = GPTC_Control(card, GCtr, IntENABLE, 1);
	
	do{
		err = GPTC_Read(card, GCtr, &Value);
        printf("encoded value: %d\n", Value);
        printf("   press any key to exit...\n");
    	usleep(100000);
	}while(!kb_hit());

	err = DO_WriteLine(card, 0, 2, 1);
    printf("Low width: %f s\n", ((float)Reg1_Val)/40000000);
	printf("Low width: %f s\n", ((float)Reg2_Val)/40000000);
	printf("press any key to exit...\n");
    getch();
	err = GPTC_Clear(card, GCtr);

/*END*/
	getch();    
    err = Release_Card(card);
    if(err < 0){
        printf("Release_Card error=%d", err);
        exit(1);
    }
 
    return 0;
}

