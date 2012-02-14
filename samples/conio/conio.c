#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include "conio.h"


char getch(void)
{
    char ch;
    int total, flag = 1;
    static struct termio ttysave;
    struct termio tty;

    if(flag){
        flag = 0;
        if(ioctl(0, TCGETA, &tty)==-1){
            perror("ioctl");
            exit(1);
        }
        ttysave = tty;
        tty.c_lflag &= ~(ICANON|ECHO|ISIG);
        tty.c_cc[VMIN] = 1;
        tty.c_cc[VTIME] = 0;
        if(ioctl(0, TCSETAF, &tty)==-1){
            restore(&ttysave);
            perror("ioctl");
            exit(2);
        }
    }

    switch(total = read(0, &ch, 1)){
        case -1:
            restore(&ttysave);
            perror("read");
            exit(3);
        case 0:
            restore(&ttysave);
            fputs("EOF error!", stderr );
            exit(4);
        default:
            ;
    }

    restore(&ttysave);
    return ch;
}


char kb_hit(void)
{
    int total, flag = 1;
    char result = 0x00;
    char temp_char;
    static struct termio ttysave;
    struct termio tty;

    if(flag){
        flag = 0;
        if(ioctl(0, TCGETA, &tty)==-1){
            perror("ioctl");
            exit(1);
        }
        ttysave = tty;
        tty.c_lflag &= ~(ICANON|ECHO|ISIG);
        tty.c_cc[VMIN] =0;
        tty.c_cc[VTIME] =1;
        if(ioctl(0, TCSETAF, &tty)==-1){
            restore(&ttysave);
            perror("ioctl");
            exit(2);
        }
    }

    switch(total = read(0, &temp_char, 1)){
        case -1:
            restore(&ttysave);
            perror("read");
            exit(3);
        case 0:
            result = 0x00;
            break;
        default:
            result = 0x01;
            break;
    }

    restore(&ttysave);
    return result;
}


void restore(struct termio *tty_save)
{
    if(ioctl(0, TCSETAF, tty_save)==-1){
        perror("ioctl");
        exit(5);
    }

    return;
}


int kbhit(void)
{
    struct timeval tv;
    fd_set rdfs;

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    FD_ZERO(&rdfs);
    FD_SET(STDIN_FILENO, &rdfs);

    select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);

   return FD_ISSET(STDIN_FILENO, &rdfs);
}
