#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include "dask.h"
#include "conio.h"

char *file_name="7300b";
U32 read_count=100000;
I16 card, err;
U8 fStop = 0;

void signal_handler(int sig_no)
{
    fStop = 1;
    printf("\nAcquisition Done...\n");
}

int main(void)
{
    U32 count=0;

    setbuf( stdout, NULL );
    clrscr();
    printf("This program inputs and outputs %ld data by PCI-7300A RevB in 10MHz\nsimultaneously (DI16DO16), and store input data to file '%s.dat'.\n\nPress any key to start the operation.\n", read_count, file_name);

    getch();
    if ((card=Register_Card (PCI_7300A_RevB, 0)) <0 ) {
        printf("Register_Card error=%d\n", card);
        exit(1);
    }
    err=DI_7300B_Config(card, 16 , TRIG_CLK_10MHZ, P7300_WAIT_NO, P7300_TERM_ON, 0, 1, 1);
    if (err!=0) {
        printf("DI_7300B_Config error=%d\n", err);
        exit(1);
    }
    // setup signal handler
    DI_EventCallBack (card, 1, 0, signal_handler);
    err = DI_ContReadPortToFile(card, 0, file_name, read_count, (F64)1/*ignored*/, ASYNCH_OP);
    if (err!=0) {
        printf("DI_ContReadPort error=%d", err);
        Release_Card( card );
        exit(1);
    }
    printf("Data acquisition ...\n");

    do{
        usleep(1000);
    }while((!fStop)&&(!kbhit()));

    if(!fStop)
        printf("\nAcquisition is manually stopped...\n");

    DI_EventCallBack (card, 0, 0, NULL);
    DI_AsyncClear(card, &count);

    printf("The input data is already stored in file '%s.dat'.\n", file_name);

    Release_Card(card);

    printf("\nPress ENTER to exit the program. "); getchar();
    return 0;
}
