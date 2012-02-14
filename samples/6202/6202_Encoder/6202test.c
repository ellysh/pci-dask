#include <stdio.h>
#include <unistd.h>
#include "dask.h"
#include "conio.h"

int main(void)
{
    I16 card = -1, card_number = 0, err;
    U32 m_data;

    if((card = Register_Card(PCI_6202, card_number))<0){
        printf("Register_Card %d error: %d", card_number, card);
        exit(-1);
    }

    err = GPTC_Clear(card, P6202_ENCODER0);
    if(err<0){
        printf("GPTC_Clear error: %d\n", err);
        exit(-1);
    }

    err = GPTC_EventSetup(card, P6202_ENCODER0, P6202_EVT_MOD_EPT,
        P6202_EPT_PULWIDTH_200us|P6202_EPT_TRGOUT_AFI, 100, 0
    );
    if(err<0){
        printf("GPTC_EventSetup error: %d\n", err);
        exit(-1);
    }

    err = GPTC_Setup(card, P6202_ENCODER0, x4_AB_Phase_Encoder, 0, 0, 0, 0);
    if(err<0){
        printf("GPTC_Setup error: %d\n", err);
        exit(-1);
    }

    do{
        clrscr();
        printf("PCI-6202 Encoder Sample :\n");
        printf("\nStart Encoder 0:\n");
        printf("--------------------------------------------------------\n");
        err = GPTC_Read(card, P6202_ENCODER0, (U32*)&m_data);
        if(err<0){
            printf("GPTC_Read error: %d\n", err);
            break;
        }
        printf(" P6202_ENCODER0: 0x%04x%04x\n", (U16)(m_data/0x10000), (U16)(m_data));
        usleep(100);
        printf(" Press Enter to stop...\n");
      }while(!kbhit());

    if(card>=0){
        err = GPTC_Clear(card, P6202_ENCODER0);
        Release_Card(card);
    }
    return 0;
}
