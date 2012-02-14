#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include "dask.h"

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    char fWriteEE;

    printf("This sample auto-calibrates itself\n\n");
    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if((card=Register_Card(PCI_9221, card_num))<0){
        printf("Register_Card error=%d", card);
        exit(1);
    }

    printf("Press any key to start auto-calibration\n");
    getch();
    printf("\nAuto-Calibrating....  it may take some minutes.\n");
    err = PCI_DB_Auto_Calibration_ALL(card);
    if(err<0){
        printf("PCI_DB_Auto_Calibration_ALL Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }
    printf("\nAuto-Calibration finish...\n");

    printf("\nWrite calibration constant to EEPROM?\n");
    printf("press 'y' for YES, and others for NO: ");
    scanf(" %c", &fWriteEE);
    if((fWriteEE=='y')||(fWriteEE=='Y')){
        err = PCI_EEPROM_CAL_Constant_Update((U16)card, EEPROM_USER_BANK1);
        if(err < 0){
            printf("Write EEPROM Error: %d\n", err);
            Release_Card(card);
            exit(1);
        }
        printf("\nWrite EEPROM finish...\n");
    }

    Release_Card(card);
    printf("Press any key to exit...\n");
    getch();

    return 0;
}
