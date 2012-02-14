/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2010/01/07                                                       */
/*                                                                            */
/* This sample performs GPTC operations.                                      */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <string.h>
#include "dask.h"

#define TIMEBASE 80000000

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 GCtr;
    U16 Mode;
    U16 SrcCtrl = 0;
    U16 PolCtrl = 0;
    U32 LReg1_Val = 0;
    U32 LReg2_Val = 0;
    U32 GPTC_Val;
    U16 STAT;
    int i = 0;
    U16 MajorSelMode;
    U16 MinorSelMode;

    printf("This sample performs GPTC operations.\n");

    printf("Card Number? ");
    scanf(" %hd", &card_num);
    card = Register_Card(PCI_9222, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    printf("GPTC Number? [0~3] ");
    scanf(" %hd", &GCtr);
    if(GCtr>3){
        printf("Invalid counter number, set to GPTC 0...\n");
        printf("Press any key to continue...\n");
        getch();
        GCtr = 0;
    }

    /*Clear GPTC*/
    err = GPTC_Clear(card, GCtr);
    if(err<0){
        printf("GPTC_Clear Error: %d\n", err);
        goto ERR_RET;
    }

    clrscr();
    printf(" \n");
    printf(" (0) Simple Gated Event Count Mode.\n");
    printf(" (1) Pulse Measurement Mode.\n");
    printf(" (2) Single Pulse Generation Mode.\n");
    printf(" (3) Pulse Train Generation Mode.\n\n");
    printf(" Which GPTC mode to be performed? [0~3] ");
    scanf(" %hd", &MajorSelMode);
    if(MajorSelMode>3){
        printf(" Invalid Mode. set to Simple Gated Event Count Mode.\n");
        printf(" Press any key to continue...\n");
        getch();
        MajorSelMode = 0;
    }

    clrscr();
    switch(MajorSelMode)
    {
        case 0:
            Mode = SimpleGatedEventCNT;
            SrcCtrl = GPTC_GATE_SRC_Ext;
            LReg1_Val = 0x7fffffff; //Initial value

            /*Setup GPTC*/
            err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, LReg1_Val, LReg2_Val);
            if(err<0){
                printf("GPTC_Setup Error: %d\n", err);
                goto ERR_RET;
            }

            /*Internal Control UpDown Source - 0: DownCount; 1: UpCount*/
            err = GPTC_Control(card, GCtr, IntUpDnCTR, 0);
            if(err<0){
                printf("GPTC_Control(IntUpDnCTR) Error: %d\n", err);
                goto ERR_RET;
            }

            /*Enable Counter*/
            err = GPTC_Control(card, GCtr, IntENABLE, 1);
            if(err<0){
                printf("GPTC_Control(IntENABLE) Error: %d\n", err);
                goto ERR_RET;
            }

            printf(" GPTC will count down from %d while GPTC/GATED pin is high...\n", LReg1_Val);
            do{
                err = GPTC_Read(card, GCtr, &GPTC_Val);
                clrscr();
                printf("/*---------------------------------*/\n");
                printf(" GPTC Number: %d\n", GCtr);
                printf(" Read Count: %d\n\n", GPTC_Val);
                printf("      You can press Enter to stop...\n");
                printf("/*---------------------------------*/\n");
                usleep(1000);
            }while(!kbhit());
            break;
        case 1:
            printf(" \n");
            printf(" (0) Pulse Period Measurement.\n");
            printf("     - Measure pulse period of a pulse from GPTC/GTAED pin.\n\n");
            printf(" (1) Pulse Width Measurement.\n");
            printf("     - Measure pulse width of a pulse from GPTC/GTAED pin.\n\n");
            printf(" (2) Pulse Edge Separation Measurement.\n");
            printf("     - Measure edge separation between pulses from GPTC/GTAED and GPTC/AUX pin.\n\n");
            printf(" Which pulse measurement mode to be performed? [0~2] ");
            scanf(" %hd", &MinorSelMode);
            if(MinorSelMode>2){
                printf(" Invalid Mode. set to Pulse Period Measurement.\n");
                printf(" Press any key to continue...\n");
                getch();
                MinorSelMode = 0;
            }
            if(MinorSelMode==0)
                Mode = SinglePeriodMSR;
            else if(MinorSelMode==1)
                Mode = SinglePulseWidthMSR;
            else
                Mode = EdgeSeparationMSR;
            SrcCtrl = GPTC_GATE_SRC_Ext; //Set External gate for measuring a pulse from GATED pin

            /*Setup GPTC*/
            err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, LReg1_Val, LReg2_Val);
            if(err<0){
                printf("GPTC_Setup Error: %d\n", err);
                goto ERR_RET;
            }

            /*Internal Control UpDown Source*/
            //Set Counter count up
            err = GPTC_Control(card, GCtr, IntUpDnCTR, 1);
            if(err<0){
                printf("GPTC_Control(IntUpDnCTR) Error: %d\n", err);
                goto ERR_RET;
            }

            /*Enable Counter*/
            err = GPTC_Control(card, GCtr, IntENABLE, 1);
            if(err<0){
                printf("GPTC_Control(IntENABLE) Error: %d\n", err);
                goto ERR_RET;
            }

            /*wait GPTC done status*/
            clrscr();
            printf(" Pulse measuring... You can press Enter to stop it...\n");
            do{
                GPTC_Status(card, GCtr, &STAT);
            }while((!((STAT>>1)&0x01))&&(!kbhit()));

            if((STAT>>1)&0x01){
                /*Read GPTC value*/
                GPTC_Read(card, GCtr, &GPTC_Val);
                printf("\n GPTC%d Value: %x\n", GCtr, GPTC_Val);

                if(MinorSelMode==0)
                    printf(" Pulse Period: %lf sec.\n", (F32)GPTC_Val/TIMEBASE);
                else if(MinorSelMode==1)
                    printf(" Pulse Width: %lf sec.\n", (F32)GPTC_Val/TIMEBASE);
                else
                    printf(" Pulse Edge Separation: %lf sec.\n", (F32)GPTC_Val/TIMEBASE);
            }
            else{
                printf("\n Pulse measurement is stopped manually...\n");
            }
            break;
        case 2:
            printf(" \n");
            printf(" (0) Single Gated Pulse Generation.\n");
            printf("     - Generate pulse to GPTC out pin while GATED pin is high.\n\n");
            printf(" (1) Single Pulse Generation with trigger.\n");
            printf("     - Generate pulse to GPTC out pin if trigger is comming from GATED pin.\n\n");
            printf(" (2) Single Pulse Generation with Re-trigger.\n");
            printf("     - Generate pulse to GPTC out pin if a trigger is comming from GATED pin.\n\n");
            printf("  Which pulse generation mode to be performed? [0~2] ");
            scanf(" %hd", &MinorSelMode);
            if(MinorSelMode>2){
                printf(" Invalid Mode. set to Single Pulse Generation.\n");
                printf(" Press any key to continue...\n");
                getch();
                MinorSelMode = 0;
            }
            if(MinorSelMode==0){
                Mode = SingleGatedPulseGen;
                SrcCtrl = GPTC_GATE_SRC_Ext;
            }
            else if(MinorSelMode==1){
                Mode = SingleTrigPulseGen;
                SrcCtrl = GPTC_GATE_SRC_Ext;
            }
            else{
                Mode = RetrigSinglePulseGen;
                SrcCtrl = GPTC_GATE_SRC_Ext;
            }
            LReg1_Val = 40000000;

            /*Setup GPTC*/
            err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, LReg1_Val, LReg2_Val);
            if(err<0){
                printf("GPTC_Setup Error: %d\n", err);
                goto ERR_RET;
            }

            /*Enable Counter*/
            clrscr();
            printf(" Press any key to enable counter...\n");
            getch();
            err = GPTC_Control(card, GCtr, IntENABLE, 1);
            if(err<0){
                printf("GPTC_Control(IntENABLE) Error: %d\n", err);
                goto ERR_RET;
            }

            if(MinorSelMode==0){
                printf("\n A %f sec. period pulse will be generated while GATED is high...\n", (F32)(LReg1_Val+LReg1_Val)/TIMEBASE);
            }
            else if(MinorSelMode==1){
                printf("\n Wait trigger to generate a %f sec. period pulse...\n", (F32)(LReg1_Val+LReg1_Val)/TIMEBASE);
            }
            else{
                printf("\n Wait retrigger to generate a %f sec. period pulse...\n", (F32)(LReg1_Val+LReg1_Val)/TIMEBASE);
            }
            printf(" Press any key to stop...\n");
            getch();
            break;
        case 3:
            printf(" \n");
            printf(" (0) Continuous Pulse Train Generation with Gated.\n");
            printf("     - Generate pulse train to GPTC/OUT pin while GATED\n");
            printf("       pin is high.\n\n");
            printf(" (1) Continuous Pulse Train Generation with trigger.\n");
            printf("     - Generate pulse train to GPTC/OUT pin if trigger is comming\n");
            printf("       from GPTC/GATED pin.\n\n");
            printf(" (2) Continuous PWM Pulse Train Generation.\n");
            printf("     - Generate PWM pulse train to GPTC/OUT pin while GATED\n");
            printf("       pin is high.\n\n");
            printf(" (3) Continuous PWM Pulse Train Generation with trigger.\n");
            printf("     - Generate PWM pulse train to GPTC/OUT pin if trigger is comming\n");
            printf("       from GPTC/GATED pin.\n\n");
            printf("  Which pulse generation mode to be performed? [0~3] ");
            scanf(" %hd", &MinorSelMode);
            if(MinorSelMode>3){
                printf(" Invalid Mode. set to Continuous Pulse Train Generation.\n");
                printf(" Press any key to continue...\n");
                getch();
                MinorSelMode = 0;
            }
            if(MinorSelMode==0){
                Mode = ContGatedPulseGen;
                SrcCtrl = GPTC_GATE_SRC_Ext;
                LReg1_Val = 40000000;
            }
            else if(MinorSelMode==1){
                Mode = SingleTrigContPulseGen;
                SrcCtrl = GPTC_GATE_SRC_Ext;
                LReg1_Val = 40000000;
            }
            else if(MinorSelMode==2){
                Mode = ContGatedPulseGenPWM;
                SrcCtrl = GPTC_GATE_SRC_Ext;
                LReg1_Val = 20000000;
                LReg2_Val = 60000000;
            }
            else{
                Mode = SingleTrigContPulseGenPWM;
                SrcCtrl = GPTC_GATE_SRC_Ext;
                LReg1_Val = 20000000;
                LReg2_Val = 60000000;
            }

            /*Setup GPTC*/
            err = GPTC_Setup(card, GCtr, Mode, SrcCtrl, PolCtrl, LReg1_Val, LReg2_Val);
            if(err<0){
                printf("GPTC_Setup Error: %d\n", err);
                goto ERR_RET;
            }

            /*Enable Counter*/
            clrscr();
            printf(" Press any key to enable counter...\n");
            getch();
            err = GPTC_Control(card, GCtr, IntENABLE, 1);
            if(err<0){
                printf("GPTC_Control(IntENABLE) Error: %d\n", err);
                goto ERR_RET;
            }

            if(MinorSelMode==0){
                F32 OutRate = (F32)TIMEBASE/(LReg1_Val+LReg1_Val);
                printf("\n Out rate: %f Hz, pulse train will be generated while GATED pin is high...\n", OutRate);
            }
            else if(MinorSelMode==1){
                F32 OutRate = (F32)TIMEBASE/(LReg1_Val+LReg1_Val);
                printf("\n Wait trigger to generate, out rate: %f Hz, pulse train...\n", OutRate);
            }
            else if(MinorSelMode==2){
                F32 Duty = (F32)LReg2_Val/(LReg1_Val+LReg2_Val);
                F32 OutRate = (F32)TIMEBASE/(LReg1_Val+LReg2_Val);
                printf("\n Out rate: %f Hz, duty cycle: %4.2f %%, pulse train will be generated while GATED pin is high...\n", OutRate, Duty);
            }
            else{
                F32 Duty = (F32)LReg2_Val/(LReg1_Val+LReg2_Val);
                F32 OutRate = (F32)TIMEBASE/(LReg1_Val+LReg2_Val);
                printf("\n Wait trigger to generate, out rate: %f Hz, duty cycle: %4.2f %%, pulse train...\n", OutRate, Duty);
            }
            printf("\n Press any key to stop...\n");
            getch();
            break;
    }

    /*Disable Counter*/
    GPTC_Control(card, GCtr, IntENABLE, 0);
ERR_RET:
    GPTC_Clear(card, GCtr);
    Release_Card(card);

    printf("\n Press any key to exit...\n");
    getch();
    return 0;
}
