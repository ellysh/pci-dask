/*----------------------------------------------------------------------------*/
/* Date: 2009/08/19                                                           */
/* Company: ADLINK Technology Inc.                                            */
/*                                                                            */
/* This sample performs auto calibration for AI and AO channels               */
/* PCI-9524 has 4 EEPROM bank to store calibration constant.                  */
/*                                                                            */
/* Default Bank - Bank 0                                                      */
/* The calibration constants of default bank will be loaded while loading     */
/* PCI-9524 driver. You can also use PCI_Load_CAL_Data() function to load     */
/* the calibration constants from the EEPROM bank manually.                   */
/*                                                                            */
/* User Defined Bank - Bank 1 ~ Bank 3                                        */
/* The three EEPROM banks support calibration constants R/W for users.        */
/* You can use PCI_Load_CAL_Data() to save the calibration constants to       */
/* a specified bank while the auto calibration have been completed.           */
/* You can also use PCI_Load_CAL_Data() function to load the calibration      */
/* constants from a specified bank manually.                                  */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <time.h>
#include "dask.h"

I16 card, err;

void ShowConstant()
{
    U16 i, j;
    U32 CalDate;
    F32 CalTemp;
    U32 ADC_offset;
    U32 ADC_gain;
    F64 Residual_offset;
    F64 Residual_scaling;
    U8 DAC_offset;
    U8 DAC_linearity;
    F32 Gain_factor;
    FILE *fin;

    if(!(fin=fopen("EEPROM_Const", "w"))){
        printf("file open error\n");
        exit(1);
    }
    for(j=0; j<16; j++){
        err = PCI9524_Acquire_AD_CalConst(card, 0, 0, j, &CalDate, &CalTemp, &ADC_offset, &ADC_gain, &Residual_offset, &Residual_scaling);
        if(err<0)
            exit(1);
        fprintf(fin, "Group:0  Gain:0  Speed:%d  CalDate:%d  CalTemp:%f  ADC_offset:%d  ADC_gain:%d  Residual_offset:%13.9lf  Residual_scaling:%13.10lf\n", j, CalDate, CalTemp, ADC_offset, ADC_gain, Residual_offset, Residual_scaling);
    }
    for(i=1; i<=4; i++){
        for(j=0; j<16; j++){
            err = PCI9524_Acquire_AD_CalConst(card, 1, i, j, &CalDate, &CalTemp, &ADC_offset, &ADC_gain, &Residual_offset, &Residual_scaling);
            if(err<0)
                exit(1);
            fprintf(fin, "Group:1  Gain:%d  Speed:%d  CalDate:%d  CalTemp:%f  ADC_offset:%d  ADC_gain:%d  Residual_offset:%13.9lf  Residual_scaling:%13.10lf\n", i, j, CalDate, CalTemp, ADC_offset, ADC_gain, Residual_offset, Residual_scaling);
        }
    }
    for(i=0; i<2; i++){
        err = PCI9524_Acquire_DA_CalConst(card, i, &CalDate, &CalTemp, &DAC_offset, &DAC_linearity, &Gain_factor);
        if(err<0)
            exit(1);
        fprintf(fin, "Channel:%d  CalDate:%d  CalTemp:%f  DAC_offset:0x%x  DAC_linearity:0x%x  Gain_factor:%f\n", i, CalDate, CalTemp, DAC_offset, DAC_linearity, Gain_factor);
    }
    fclose(fin);
}


int main(int argc, char **argv)
{
    U16 card_num;
    U8 fSave;
    U16 bank;

    printf("The sample performs auto calibration for AI and AO channels.\n");
    printf("Card Number? ");
    scanf(" %hd", &card_num);

    card = Register_Card(PCI_9524, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    printf("Press any key to start auto calibration.\n");
    printf("It may take some minutes to hang your system... (about 20 minutes)\n");
    getch();

    err = PCI_DB_Auto_Calibration_ALL(card);
    if(err<0){
        printf("Calibration Error: %d\n", err);
        Release_Card(card);
    }
    //You can use the folling function to load calibrated constants from EEPROM
    //PCI_Load_CAL_Data(card, 0);

    printf("\nAuto Calibration have been done successfully.\n");
    ShowConstant();

    printf("\nSave the Calibration constants to the EEPROM? [Y]es or [N]o : ");
    scanf(" %c", &fSave);
    if((fSave=='Y')||(fSave=='y')){
BANK_S:
        printf("Bank Number? [1~3] : ");
        scanf(" %hd", &bank);
        if((bank==0)||(bank>3)){
            printf("Invalid Bank.\n");
            goto BANK_S;
        }
        else{
            err = PCI_EEPROM_CAL_Constant_Update(card, bank);
            if(err<0){
                printf("PCI_Load_CAL_Data Error: %d\n", err);
            }

        }
    }

    printf("Press any key to exit...\n");
    getch();
    Release_Card(card);
    return 0;
}
