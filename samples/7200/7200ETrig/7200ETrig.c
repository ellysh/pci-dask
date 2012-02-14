#include <stdio.h>
#include <errno.h>
#include "conio.h"
#include "dask.h"


int main(void)
{
    I16 card;

    if((card = Register_Card(PCI_7200, 0))<0){
        printf("Register_Card error=%d\n", card);
        exit(1);
    }

    DO_WriteExtTrigLine(card, 1);
    getchar();
    //DO_WriteExtTrigLine (card, 0);
    Release_Card(card);
    return 0;
}
