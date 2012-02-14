#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "dask.h"

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U32 out_data;
    U16 rb_data;
    U16 read_data;
    int i;
    
    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if((card=Register_Card (PCI_9221, card_num)) < 0) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

    out_data = 1;
    i = 0;
    do{
        clrscr();
        //Write Data to DO Line 
	err =  DO_WriteLine(card, 0, i, out_data);
        if(err < 0){
            printf("DO Write Line%d Error: %d\n", i, err);
	    exit(1);
        }
        printf("DO Write Data to Line%d: 0x%x\n", i, (unsigned int)out_data);
        
	//Read Back Data from DO Line
	err =  DO_ReadLine(card, 0, 0, &rb_data);
        if(err < 0){
            printf("DO Read Line0 Error: %d\n", err);
	    exit(1);
        }
        printf("DO Read Back from Line0: 0x%x\n", (unsigned int)rb_data);
	
	err =  DO_ReadLine(card, 0, 1, &rb_data);
        if(err < 0){
            printf("DO Read Line1 Error: %d\n", err);
	    exit(1);
        }
        printf("DO Read Back from Line1: 0x%x\n", (unsigned int)rb_data);
	
	err =  DO_ReadLine(card, 0, 2, &rb_data);
        if(err < 0){
            printf("DO Read Line2 Error: %d\n", err);
	    exit(1);
        }
        printf("DO Read Back from Line2: 0x%x\n", (unsigned int)rb_data);
	
	err =  DO_ReadLine(card, 0, 3, &rb_data);
        if(err < 0){
            printf("DO Read Line3 Error: %d\n", err);
	    exit(1);
        }
        printf("DO Read Back from Line3: 0x%x\n", (unsigned int)rb_data);
        
	//Read Data from DI Line
        err =  DI_ReadLine(card, 0, 0, &read_data);
        if(err < 0){
            printf("DI Read Line0 Error: %d\n", err);
	    exit(1);
        }
        printf("DI Read Data from Line0: 0x%x\n", (unsigned int)read_data);
	
        err =  DI_ReadLine(card, 0, 1, &read_data);
        if(err < 0){
            printf("DI Read Line1 Error: %d\n", err);
	    exit(1);
        }
        printf("DI Read Data from Line1: 0x%x\n",  (unsigned int)read_data);

        err =  DI_ReadLine(card, 0, 2, &read_data);
        if(err < 0){
            printf("DI Read Line2 Error: %d\n", err);
	    exit(1);
        }
        printf("DI Read Data from Line2: 0x%x\n", (unsigned int)read_data);
	
        err =  DI_ReadLine(card, 0, 3, &read_data);
        if(err < 0){
            printf("DI Read Line3 Error: %d\n", err);
	    exit(1);
        }
        printf("DI Read Data from Line3: 0x%x\n", (unsigned int)read_data);

	if((++i)==4){
	    i=0;
	    out_data = (!out_data) ? 1:0;
	}
	printf("Press any key to exit...\n");
	usleep(200000);
    }while(!kb_hit());
    
    err = Release_Card(card);
    if(err < 0){
        printf("Release_Card error=%d", err);
        exit(1);
    }
 
    return 0;
}
