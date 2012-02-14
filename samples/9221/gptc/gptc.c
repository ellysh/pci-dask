#include <stdio.h>
#include <stdlib.h>
#include "dask.h"
#include "conio.h"

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U16 func_sel;
    U16 gptc_Ctr = 0;
    U16 gptc_Mode = 0;
    U16 gptc_SrcCtrl = 0;
    U16 gptc_PolCtrl = 0;
    U32 gptc_Reg1Val = 0;
    U32 gptc_Reg2Val = 0;
    U16 gptc_Status = 0;
    U32 gptc_CtrVal = 0;

    printf("\nThis sample performs some general purpose timer/counter function.\n\n");
    printf("Please input a card number: ");
    scanf(" %hd", &card_num);
    if((card=Register_Card (PCI_9221, card_num))<0){
        printf("Register_Card error=%d", card);
        exit(1);
    }

FunSel:
    printf("\n(0) Edge Counting");
    printf("\n(1) Pulse Measurement");
    printf("\n(2) Single Pulse Generatation");
    printf("\n(3) Pulse Train Generation");
    printf("\n(4) Encoder Mode");
    printf("\nPlease Select GPTC Function: ");
    scanf(" %hd", &func_sel);
    printf("\n");
    switch(func_sel){
        case 0: //edge counting
            printf("\nCounter will down-count while external gate is high and external clock is comming.\n");
            printf("Press any key to start.\n");
            getch();
            gptc_Mode = SimpleGatedEventCNT;
            gptc_SrcCtrl = GPTC_CLK_SRC_Ext|GPTC_GATE_SRC_Ext|GPTC_UPDOWN_Int;
            gptc_Reg1Val = 0xffffffff; //sets counter initial value
            GPTC_Clear(card, gptc_Ctr);
            GPTC_Setup(card, gptc_Ctr, gptc_Mode, gptc_SrcCtrl, gptc_PolCtrl, gptc_Reg1Val, gptc_Reg2Val);
            GPTC_Control(card, gptc_Ctr, IntUpDnCTR, 0); //lets counter count down
            GPTC_Control(card, gptc_Ctr, IntENABLE, 1); //enable counter
            do{
                clrscr();
                GPTC_Read(card, gptc_Ctr, &gptc_CtrVal);
                printf("\nGPTC%d Value: 0x%x\n", gptc_Ctr, gptc_CtrVal);
                printf("                      Press Enter to stop\n");
                usleep(10000);
            }while(!kbhit());
            GPTC_Clear(card, gptc_Ctr);
            break;
        case 1: //measure pulse width and length
        {
            U8 fMSR;
            printf("\nMeasure Pulse (W)idth or Pulse (P)eriod? ([W]or[P]):[P] ");
            scanf(" %c", &fMSR);
            if((fMSR=='W')||(fMSR=='w')){ //Measure pulse width
                printf("\nPress any key to start pulse width measurement\n");
                getch();
                gptc_Mode = SinglePulseWidthMSR;
                gptc_SrcCtrl = GPTC_GATE_SRC_Ext; //for measured pulse coming
                GPTC_Clear(card, gptc_Ctr);
                GPTC_Setup(card, gptc_Ctr, gptc_Mode, gptc_SrcCtrl, gptc_PolCtrl, gptc_Reg1Val, gptc_Reg2Val);
                GPTC_Control(card, gptc_Ctr, IntUpDnCTR, 1);
                GPTC_Control(card, gptc_Ctr, IntENABLE, 1);
                printf("Wait the measured pulse from external gate\n");
                do{
                    GPTC_Status(card, gptc_Ctr, &gptc_Status);
                    usleep(1);
                }while(!((gptc_Status>>1)&0x1));
                printf("\nPulse Width Measurement Finished...\n");
                err = GPTC_Read(card, gptc_Ctr, &gptc_CtrVal);
                printf("\nGPTC%d Value: 0x%x\n", gptc_Ctr, gptc_CtrVal);
                printf("Pulse Width %f s\n", ((float)gptc_CtrVal)/40000000);
                GPTC_Clear(card, gptc_Ctr);
                printf("\nPress any key to exit\n");
                getch();
            }
            else{ //Measure pulse period
                printf("\nPress any key to start pulse period measurement\n");
                getch();
                gptc_Mode = SinglePeriodMSR;
                gptc_SrcCtrl = GPTC_GATE_SRC_Ext; //for measured pulse comming
                GPTC_Clear(card, gptc_Ctr);
                GPTC_Setup(card, gptc_Ctr, gptc_Mode, gptc_SrcCtrl, gptc_PolCtrl, gptc_Reg1Val, gptc_Reg2Val);
                GPTC_Control(card, gptc_Ctr, IntUpDnCTR, 1);
                GPTC_Control(card, gptc_Ctr, IntENABLE, 1);
                printf("Wait the measured pulse from external gate\n");
                do{
                    GPTC_Status(card, gptc_Ctr, &gptc_Status);
                    usleep(1);
                }while(!((gptc_Status>>1)&0x1));
                printf("\nPulse Period Measurement Finished...\n");
                err = GPTC_Read(card, gptc_Ctr, &gptc_CtrVal);
                printf("\nGPTC%d Value: 0x%x\n", gptc_Ctr, gptc_CtrVal);
                printf("Pulse Period %f s\n", ((float)gptc_CtrVal)/40000000);
                GPTC_Clear(card, gptc_Ctr);
                printf("\nPress any key to exit\n");
                getch();
            }
            break;
        }
        case 2: //Single pulse generation
        {
            U16 fTrig;
            U8 fRetrig;
            F32 pulse_period;
            printf("\n\n(0) Gated Pulse Generation - \n");
            printf("\tPulse will be generated while GPTC_Gate is high.\n");
            printf("(1) Triggered Pulse Generation - \n");
            printf("\tPulse will be generated while GPTC_Gate from low to high.\n");
            printf("Gated or Triggered? ([0]or[1]):[1] ");
            scanf(" %d", &fTrig);
            if(fTrig==0){ //Gated
                printf("\nPlease input the pulse period to be generated while GPTC_Gate is high: (ms) ");
                scanf(" %f", &pulse_period);
                gptc_Reg1Val = 40000*pulse_period*0.5; //sets pulse length to be generated
                gptc_Mode = SingleGatedPulseGen;
                gptc_SrcCtrl = GPTC_GATE_SRC_Ext;
                GPTC_Clear(card, gptc_Ctr);
                GPTC_Setup(card, gptc_Ctr, gptc_Mode, gptc_SrcCtrl, gptc_PolCtrl, gptc_Reg1Val, gptc_Reg2Val);
                GPTC_Control(card, gptc_Ctr, IntENABLE, 1);
                printf("\nPulse Period is %4.2f ms\n", pulse_period);
                printf("press any key to exit.\n");
                getch();
                GPTC_Clear(card, gptc_Ctr);
            }
            else{ //Triggered
                printf("\nRe-Trigger? ([N]or[Y]):[Y]");
                scanf(" %c", &fRetrig);
                if((fRetrig=='n')||(fRetrig=='N')){
                    printf("\nPlease input the pulse period to be generated while GPTC_Gate is from low to high: (ms) ");
                    scanf(" %f", &pulse_period);
                    gptc_Reg1Val = 40000*pulse_period*0.5;
                    gptc_Mode = SingleTrigPulseGen;
                    gptc_SrcCtrl = GPTC_GATE_SRC_Ext;
                    GPTC_Clear(card, gptc_Ctr);
                    GPTC_Setup(card, gptc_Ctr, gptc_Mode, gptc_SrcCtrl, gptc_PolCtrl, gptc_Reg1Val, gptc_Reg2Val);
                    GPTC_Control(card, gptc_Ctr, IntENABLE, 1);
                    printf("\nPulse Period is %4.2f ms\n", pulse_period);
                    printf("press any key to exit.\n");
                    getch();
                    GPTC_Clear(card, gptc_Ctr);
                }
                else{ //Re-Trigger
                    printf("\nPlease input the pulse period to be generated while GPTC_Gate is from low to high: (ms) ");
                    scanf(" %f", &pulse_period);
                    gptc_Reg1Val = 40000*pulse_period*0.5;
                    gptc_Mode = RetrigSinglePulseGen;
                    gptc_SrcCtrl = GPTC_GATE_SRC_Ext;
                    GPTC_Clear(card, gptc_Ctr);
                    GPTC_Setup(card, gptc_Ctr, gptc_Mode, gptc_SrcCtrl, gptc_PolCtrl, gptc_Reg1Val, gptc_Reg2Val);
                    GPTC_Control(card, gptc_Ctr, IntENABLE, 1);
                    printf("\nPulse Period is %4.2f ms\n", pulse_period);
                    printf("press any key to exit.\n");
                    getch();
                    GPTC_Clear(card, gptc_Ctr);
                }
            }
            break;
        }
        case 3: //Pulse Train Generation
        {
            U16 fTrig;
            F32 pulse_period;
            F32 dutyCycle;
            printf("\n\n(0) Gated Pulse Generation - \n");
            printf("\tPulse train will be generated while GPTC_Gate is high.\n");
            printf("(1) Triggered Pulse Generation - \n");
            printf("\tPulse train will be generated while GPTC_Gate from low to high.\n");
            printf("Gated or Triggered? ([0]or[1]):[1] ");
            scanf(" %d", &fTrig);
            if(fTrig==0){ //Gated
                printf("\nPlease input the pulse period to be generated while GPTC_Gate is high: (ms) ");
                scanf(" %f", &pulse_period);
                printf("Please input the Duty Cycle of pulse train to be generated (%): ");
                scanf(" %f", &dutyCycle);
                gptc_Reg2Val = 40000*pulse_period*(dutyCycle/100);
                gptc_Reg1Val = 40000*pulse_period*(1-dutyCycle/100);
                gptc_Mode = ContGatedPulseGenPWM;
                gptc_SrcCtrl = GPTC_GATE_SRC_Ext;
                GPTC_Clear(card, gptc_Ctr);
                GPTC_Setup(card, gptc_Ctr, gptc_Mode, gptc_SrcCtrl, gptc_PolCtrl, gptc_Reg1Val, gptc_Reg2Val);
                GPTC_Control(card, gptc_Ctr, IntENABLE, 1);
                printf("\nPulse Period is %4.2f ms\n", pulse_period);
                printf("\nPulse dutycycle is %4.2f %\n", dutyCycle);
                printf("press any key to exit.\n");
                getch();
                GPTC_Clear(card, gptc_Ctr);
            }
            else{ //Triggered
                printf("\nPlease input the pulse period to be generated while GPTC_Gate is from low to high: (ms) ");
                scanf(" %f", &pulse_period);
                printf("Please input the Duty Cycle of pulse train to be generated (%): ");
                scanf(" %f", &dutyCycle);
                gptc_Reg2Val = 40000*pulse_period*(dutyCycle/100);
                gptc_Reg1Val = 40000*pulse_period*(1-dutyCycle/100);
                gptc_Mode = SingleTrigContPulseGenPWM;
                gptc_SrcCtrl = GPTC_GATE_SRC_Ext;
                GPTC_Clear(card, gptc_Ctr);
                GPTC_Setup(card, gptc_Ctr, gptc_Mode, gptc_SrcCtrl, gptc_PolCtrl, gptc_Reg1Val, gptc_Reg2Val);
                GPTC_Control(card, gptc_Ctr, IntENABLE, 1);
                printf("\nPulse Period is %4.2f ms\n", pulse_period);
                printf("\nPulse dutycycle is %4.2f %\n", dutyCycle);
                printf("press any key to exit.\n");
                getch();
                GPTC_Clear(card, gptc_Ctr);
            }
            break;
        }
        case 4: //Encoder Mode
        {
            U16 fEnMode;
            printf("\n\n(0) CW/CCW Mode\n");
            printf("(1) x1 AB Phase Mode\n");
            printf("(2) x2 AB Phase Mode\n");
            printf("(3) x4 AB Phase Mode\n");
            printf("Which Enconder Mode? ([0],[1],[2],or[3]):[0] ");
            scanf(" %hd", &fEnMode);
            switch(fEnMode){
                case 0:
                default:
                    gptc_Mode = CW_CCW_Encoder;
                    break;
                case 1:
                    gptc_Mode = x1_AB_Phase_Encoder;
                    break;
                case 2:
                    gptc_Mode = x2_AB_Phase_Encoder;
                    break;
                case 3:
                    gptc_Mode = x4_AB_Phase_Encoder;
                    break;
            }
            printf("\nPress any key to start encode.\n");
            getch();
            GPTC_Clear(card, gptc_Ctr);
            GPTC_Setup(card, gptc_Ctr, gptc_Mode, gptc_SrcCtrl, gptc_PolCtrl, gptc_Reg1Val, gptc_Reg2Val);
            GPTC_Control(card, gptc_Ctr, IntENABLE, 1);
            do{
                clrscr();
                GPTC_Read(card, gptc_Ctr, &gptc_CtrVal);
                printf("\nGPTC%d Value: 0x%x\n", gptc_Ctr, gptc_CtrVal);
                printf("                      Press Enter to stop\n");
                usleep(100000);
            }while(!kbhit());
            GPTC_Clear(card, gptc_Ctr);
            break;
        }
        default:
        {
            U8 fExit;
            printf("Invalid Function Seleted\n");
            printf("(E)xit or (R)e-Select? ([E]or[R]):[R]");
            scanf(" %c", &fExit);
            if((fExit=='E')||(fExit=='e'))
                break;
            else{
                clrscr();
                goto FunSel;
            }
        }
    }
    err = Release_Card(card);
    if(err < 0){
        printf("Release_Card error=%d", err);
        exit(1);
    }

    return 0;
}
