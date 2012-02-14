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

    err = GPTC_Clear(card, P6202_GPTC0);
    if(err<0){
        printf("GPTC_Clear error: %d\n", err);
        exit(-1);
    }

    err = GPTC_Setup(card, P6202_GPTC0 , SingleGatedPulseGen,
        GPTC_CLK_SRC_Int|GPTC_GATE_SRC_Int|GPTC_UPDOWN_Int,
        GPTC_GATE_HACTIVE|GPTC_UPDOWN_HACTIVE|GPTC_CLKSRC_HACTIVE,
        80000000, 40000000
    );
    if(err<0){
        printf("GPTC_Setup error: %d\n", err);
        exit(-1);
    }

    err = GPTC_Control(card, P6202_GPTC0, IntENABLE, 1);
    if(err<0){
        printf("GPTC_Control error: %d\n", err);
        exit(-1);
    }

    do{
        clrscr();
        printf("PCI-6202 GPTC Sample :\n");
        printf("\nStart GPTC 0:\n");
        printf("--------------------------------------------------------\n");
        err = GPTC_Read(card, P6202_GPTC0, (U32*)&m_data);
        if(err<0){
            printf("GPTC_Read error: %d\n", err);
            break;
        }
        printf(" P6202_GPTC0: 0x%04x%04x\n", (U16)(m_data/0x10000), (U16)(m_data));
        usleep(100);
        printf(" Press Enter to stop...\n");
      } while(!kbhit());

    if(card>=0){
        err = GPTC_Clear(card, P6202_GPTC0);
        Release_Card(card);
    }
    return 0;
}
