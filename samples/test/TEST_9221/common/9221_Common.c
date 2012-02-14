#include <stdio.h>
#include <stdlib.h>
#include "dask.h"

int main(int argc, char **argv)
{
    I16 card, err;
    U16 card_num;
    U32 addr_io, addr_mem;
    U32 mapped_mem_addr;

    printf("Please input a card number: ");
    scanf(" %hd", &card_num);

    if((card=Register_Card (PCI_9221, card_num)) < 0) {
        printf("Register_Card error=%d", card);
        exit(1);
    }

    err =  GetBaseAddr(card, &addr_io, &addr_mem);
    if(err < 0){
        printf("GetBaseAddr error: %d\n", err);
	exit(1);
    }
    printf("IO Addr: 0x%x, MEM_IO Addr: 0x%x\n", (unsigned int)addr_io, (unsigned int)addr_mem);
    
    err =  GetLCRAddr(card, &mapped_mem_addr);
    if(err < 0){
        printf("GetLCRAddr error: %d\n", err);
	exit(1);
    }
    printf("Mapped MEM_IO Addr: 0x%x\n", (unsigned int)mapped_mem_addr);	  
   
    printf("INTCSR: 0x%x\n", *((unsigned int*)(mapped_mem_addr)));
    
    err = Release_Card(card);
    if(err < 0){
        printf("Release_Card error=%d", err);
        exit(1);
    }
 
    return 0;
}

