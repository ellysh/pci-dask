#include <stdio.h>
#include <stdlib.h>
#include "conio.h"
#include "dask.h"

void main(void)
{

	I16 card, err, card_num=0;
	U32 out_value = 0;
	U32 read_out_value;
	U32 in_value;
	U32 out_value_array[4] = {1, 1, 1, 1};
	int i=0;
	char flag;
	int j=0;
	int k=1;
	int l=0;
	int port, line, bit;
	U32 out0[4] = {0, 0, 0, 0};
	U32 out1[4] = {0x1, 0x1, 0x1, 0x1};
	
	
    printf("This program outputs data to DO Port 0/1, and inputs data from DI Port 0/1\n");
	printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if ((card=Register_Card (PCI_7444, card_num)) <0 ) {
       	printf("Register_Card error=%d", card);
       	exit(1);
    }

   	do{
		clrscr();
       	printf ("-------------------------------------------------\n");
		
//printf("\ntype port number:");
//scanf(" %d", &port);
//printf("\ntype line number:");
//scanf(" %d", &line);
//printf("\ntype out bit:");
//scanf(" %d", &bit);
//if(port == 0){
//	if(bit&0x1)
//		DO_WritePort(card, port, 0x1);
//	else
//		DO_WritePort(card, port, 0);
//}
//else if(port==1){
//	if(bit&0x1)
//		DO_WritePort(card, port, 0x30);
//	else
//		DO_WritePort(card, port, 0);
//}
//else if(port==2){
//	if(bit&0x1)
//		DO_WritePort(card, port, 0x700);
//	else
//		DO_WritePort(card, port, 0);
//}
//else if(port==3){
//	if(bit&0x1)
//		DO_WritePort(card, port, 0xf000);
//	else
//		DO_WritePort(card, port, 0);
//}
i++;
if(i%2){
	printf("write 1\n");
	DO_SimuWritePort(card, 4, out1);
	//DO_WritePort(card, 0, 0x1);
	//usleep(1000000);
//	DO_WritePort(card, 0, 0x3);
}
else{
	printf("write 0\n");
	DO_SimuWritePort(card, 4, out0);
//	DO_WritePort(card, 0, 0x0);
}	
//		out_value = 1<<i;
//		out_value_array[0] = 1<<i;
//		out_value_array[1] = 1<<i;
//		out_value_array[2] = 1<<i;
//		out_value_array[3] = 1<<i;

/*		err = DO_WritePort(card, P7444_CH0, out_value);
		printf("err: %d, out_value: 0x%x\n", err, out_value);

		err = DO_WritePort(card, P7444_CH1, out_value);
		printf("err: %d, out_value: 0x%x\n", err, out_value);

		err = DO_WritePort(card, P7444_CH2, out_value);
		printf("err: %d, out_value: 0x%x\n", err, out_value);

		err = DO_WritePort(card, P7444_CH3, out_value);
		printf("err: %d, out_value: 0x%x\n", err, out_value);
*/
//		err = DO_SimuWritePort(card, 4, out_value_array);
//		printf("err: %d\n", err);

//		err = DO_WriteLine(card, P7444_CH0, j, k%2);
//		printf("err: %d\n", err);
//		err = DO_WriteLine(card, P7444_CH1, j, k%2);
//		printf("err: %d\n", err);
//		err = DO_WriteLine(card, P7444_CH2, j, k%2);
//		printf("err: %d\n", err);
//		err = DO_WriteLine(card, P7444_CH3, j, k%2);
//		printf("err: %d\n", err);

//		err = DO_ReadPort(card, P7444_CH0, &read_out_value);
//		err = DO_ReadLine(card, P7444_CH0, l, &read_out_value); 
//		printf("err: %d, read_out_value: 0x%x\n", err, read_out_value);

//		err = DO_ReadPort(card, P7444_CH1, &read_out_value);
//		err = DO_ReadLine(card, P7444_CH1, l+1, &read_out_value);
//		printf("err: %d, read_out_value: 0x%x\n", err, read_out_value);

//		err = DO_ReadPort(card, P7444_CH2, &read_out_value);
//		err = DO_ReadLine(card, P7444_CH2, l+2, &read_out_value);
//		printf("err: %d, read_out_value: 0x%x\n", err, read_out_value);

//		err = DO_ReadPort(card, P7444_CH3, &read_out_value);
//		err = DO_ReadLine(card, P7444_CH3, l+3, &read_out_value);
//		printf("err: %d, read_out_value: 0x%x\n", err, read_out_value);
		
//		if((i%4)==3){
//			l+=4;
//			if(l==32)
//				l=0;
//		}
//		i++;
//		j++;
//		if(i==32)
//			i=0;	
//		if(j==32){
//			j=0;
//			k++;
//		}
		printf("\n\nPress any key to start\n");
		flag = getch();

	} while(/*!kb_hit()*/flag!='q');

	Release_Card(card);
}
