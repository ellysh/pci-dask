#include <stdio.h>
#include <stdlib.h>
#include "conio.h"
#include "dask.h"

void main(void)
{

	I16 card, err, card_num=0;
	U8 status;
	U32 DO_RB_Data;

    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if ((card=Register_Card (PCI_7444, card_num)) <0 ) {
       	printf("Register_Card error=%d", card);
       	exit(1);
    }

/*	err = DO_ReadPort(card, P7444_CH0, &DO_RB_Data);
	printf("err: %d, DO CH0 ReadBack: %x\n", err, DO_RB_Data);	
	err = DO_ReadPort(card, P7444_CH1, &DO_RB_Data);
	printf("err: %d, DO CH1 ReadBack: %x\n", err, DO_RB_Data);	
	err = DO_ReadPort(card, P7444_CH2, &DO_RB_Data);
	printf("err: %d, DO CH2 ReadBack: %x\n", err, DO_RB_Data);	
	err = DO_ReadPort(card, P7444_CH3, &DO_RB_Data);
	printf("err: %d, DO CH3 ReadBack: %x\n", err, DO_RB_Data);

	err = HotResetHoldControl(card, HRH_OFF);
	if(err < 0){
		printf("HotResetHoldControl Error: %d\n", err);
		exit(1);
	}
	err = HotResetHoldStatus(card, &status);
	if(err < 0){
		printf("HotResetHoldControl Error: %d\n", err);
		exit(1);
	}

	err = DO_WritePort(card, P7444_CH0, 0xff);
	//printf("err: %d, DO CH0 : %x\n", err, DO_RB_Data);	
	err = DO_WritePort(card, P7444_CH1, 0xffff);
	//printf("err: %d, DO CH1 ReadBack: %x\n", err, DO_RB_Data);	
	err = DO_WritePort(card, P7444_CH2, 0xffffff);
	//printf("err: %d, DO CH2 ReadBack: %x\n", err, DO_RB_Data);	
	err = DO_WritePort(card, P7444_CH3, 0xffffffff);
	//printf("err: %d, DO CH3 ReadBack: %x\n", err, DO_RB_Data);
*/
	{
		U32 BaseAddr, BaseAddr2, LcrAddr;
		U16 cardType;

		err = GetBaseAddr(card, &BaseAddr, &BaseAddr2);
		printf("BaseAddr: %x, BaseAddr2: %x\n", BaseAddr, BaseAddr2);
		err = GetLCRAddr(card, &LcrAddr);
		printf("LcrAddr: %x\n", LcrAddr);
		err = GetCardType(card, &cardType);
		printf("cardType: %d\n", cardType);
	}
	Release_Card(card);
}
