#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>
#include "dask.h"
#include "conio.h"


U16 DoBuf[100000]; // 100K word 200K byte
char *file_name="7300b";
U32 read_count=100000;

int main(void)
{
    I16 card, err;
    BOOLEAN bStopped;
    U32 count, i;

    setbuf( stdout, NULL );
    clrscr();
    printf("This program inputs and outputs %ld data by PCI-7300A RevB in 10MHz\nsimultaneously (DI16DO16), and store input data to file '%s.dat'.\n\nPress any key to start the operation.\n", read_count, file_name);

    getch();
    if ((card=Register_Card (PCI_7300A_RevB, 0)) <0 ) {
        printf("Register_Card error=%d\n", card);
        exit(1);
    }

    for (i=0; i<read_count; i++)
        DoBuf[i] = (U16) i;

    err=DI_7300B_Config(card, 16 , TRIG_CLK_10MHZ, P7300_WAIT_NO, P7300_TERM_ON, 0, 1, 1);
    if (err!=0) {
        printf("DI_7300B_Config error=%d\n", err);
        exit(1);
    }

    err=DO_7300B_Config (card, 16, TRIG_CLK_10MHZ, P7300_WAIT_NO, P7300_TERM_ON, 0, 0x40004000);

    if (err!=0) {
        printf("DO_7300B_Config error=%d\n", err);
        exit(1);
    }

    err = DO_ContWritePort(card, 0, DoBuf, read_count, 6, (F64)1, ASYNCH_OP);
    if (err!=0) {
        printf("DO_ContWritePort error=%d", err);
        Release_Card( card );
        exit(1);
    }

    err = DI_ContReadPortToFile(card, 0, file_name, read_count, (F64)1/*ignored*/, ASYNCH_OP);
    if (err!=0) {
        printf("DI_ContReadPort error=%d", err);
        DO_AsyncClear(card, &count);
        Release_Card( card );
        exit(1);
    }

    do {
        DO_AsyncCheck(card, &bStopped, &count);
    } while (!bStopped);

    do {
        DI_AsyncCheck(card, &bStopped, &count);
    } while (!bStopped);

    DI_AsyncClear(card, &count);
    DO_AsyncClear(card, &count);

    printf("\n\nThe input data is already stored in file '%s.dat'.\n", file_name);

    Release_Card(card);
    printf("\nPress ENTER to exit the program. "); getchar();
    return 0;
}
