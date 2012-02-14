/*----------------------------------------------------------------------------*/
/* Company : ADLINK                                                           */
/* Date    : 2010/01/06                                                       */
/*                                                                            */
/* This sample performs AI/AO auto calibration for PCI-9222.                  */
/* There are two EEPROM banks, default bank and user bank, to store AI/AO     */
/* calibration constants. The driver loads calibration constants and writes   */
/* them to ADC/DAC during the period of driver loaded.                        */
/* User can use PCI_DB_Auto_Calibration_ALL() to perform auto calibration,    */
/* and those auto calibration constants will be written to ADC/DAC            */
/* automatically. Then, User can use PCI_EEPROM_CAL_Constant_Update() to      */
/* store the auto calibration constants to user bank of EEPROM.               */
/* PCI_Load_CAL_Data() is used to load the calibration constants from default */
/* or user bank of EEPROM to ADC/DAC.                                         */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "dask.h"


int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U8 fWriteEE;

    printf("The sample performs AI/AO auto calibration.\n");

    printf("Card Number: ");
    scanf(" %hd", &card_num);
    card = Register_Card(PCI_9222, card_num);
    if(card<0){
        printf("Register_Card Error: %d\n", card);
        exit(1);
    }

    printf("Press any key to start AI/AO auto calibration.\n");
    getch();
    printf("Auto Calibration is performing... It will take some minites...\n");
    err = PCI_DB_Auto_Calibration_ALL(card);
    if(err<0){
        printf("Auto Calibration Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    printf("\nAuto Calibration is done...\n");

    printf("\nStore the auto calibration constants to EEPROM?\n");
    printf("Press 'Y' to store them to EEPROM, and others to exit.\n");
    fWriteEE = getch();
    if((fWriteEE=='Y')||(fWriteEE=='y')){
        printf("Store auto calibration constants to user bank of EEPROM...\n");
        err = PCI_EEPROM_CAL_Constant_Update(card, EEPROM_USER_BANK1);
        if(err<0){
            printf("Save Calibration Constants to EEProm Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        printf("\nAuto calibration constants are stored to EEPROM completely.\n");
    }

    printf("\nPress any key to exit...\n");
    getch();
    Release_Card(card);
    return 0;
}
