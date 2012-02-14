#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>
#include "dask.h"

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;

    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if((card=Register_Card(PCI_9221, card_num)) < 0) {
        printf("Register_Card error=%d", card);
        exit(1);
    }	

    printf("---------- EEPROM_DEFAULT_BANK -------------\n");
    err = PCI_Load_CAL_Data((U16)card, EEPROM_DEFAULT_BANK);
    if(err<0){
        printf("PCI_Load_CAL_Data Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    printf("---------- EEPROM_USER_BANK1 -------------\n");
    err = PCI_Load_CAL_Data((U16)card, EEPROM_USER_BANK1);
    if(err<0){
        printf("PCI_Load_CAL_Data Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

	printf("---------- AUTO K -------------\n");
    err = PCI_DB_Auto_Calibration_ALL((U16)card);
    if(err<0){
        printf("PCI_DB_Auto_Calibration_ALL Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

	printf("---------- Write Constant to EEPROM -------------\n");
    err = PCI_EEPROM_CAL_Constant_Update((U16)card, EEPROM_USER_BANK1);
    if(err<0){
        printf("PCI_EEPROM_CAL_Constant_Update Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    printf("---------- EEPROM_USER_BANK1 -------------\n");
    err = PCI_Load_CAL_Data((U16)card, EEPROM_USER_BANK1);
    if(err<0){
        printf("PCI_Load_CAL_Data Error: %d\n", err);
        Release_Card(card);
        exit(1);
    }

    Release_Card(card);
    printf("Press any key to exit...\n");
    getch();

    return 0;
}
