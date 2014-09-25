#include <stdio.h>
#include "strcat_opt.h"

char *strcat_opt(char *dest, const char *src) {

    unsigned long long int buffer;
    char *tmp = dest;

    //the goal of this loop is to find the \0 in dest
    while (1) {
        buffer = *((unsigned long long int *) dest);
        if ((buffer & 0x00000000000000FF)  == 0)
            break;
        if ((buffer & 0x000000000000FF00) == 0) {
            dest = dest + 1;
            break;
        }
        if ((buffer & 0x0000000000FF0000) == 0) {
            dest = dest + 2;
            break;
        }
        if ((buffer & 0x00000000FF000000) == 0) {
            dest = dest + 3;
            break;
        }
        if ((buffer & 0x000000FF00000000) == 0) {
            dest = dest + 4;
            break;
        }
        if ((buffer & 0x0000FF0000000000) == 0) {
            dest = dest + 5;
            break;
        }
        if ((buffer & 0x00FF000000000000) == 0) {
            dest = dest + 6;
            break;
        }
        if ((buffer & 0xFF00000000000000) == 0) {
            dest = dest + 7;
            break;
        }
        dest = dest + 8;
    }

    while (1) { 
        buffer = *((unsigned long long int *) src);
        //printf("buffer = %llx\n", buffer);
        if ((buffer & 0x00000000000000FF)  == 0) {
            *dest = '\0';
            break;
        }
        if ((buffer & 0x000000000000FF00) == 0) {
            *dest = *src;
            *(dest+1) = '\0';
            break;
        }
        if ((buffer & 0x0000000000FF0000) == 0) {
            *dest = *src;
            *(dest+1) = *(src+1);
            *(dest+2) = '\0';
            //printf("dest = %s\n", tmp);
            break;
        }
        if ((buffer & 0x00000000FF000000) == 0) {
            *dest = *src;
            *(dest+1) = *(src+1);
            *(dest+2) = *(src+2);
            *(dest+3) = '\0';
            break;
        }
        if ((buffer & 0x000000FF00000000) == 0) {
            *dest = *src;
            *(dest+1) = *(src+1);
            *(dest+2) = *(src+2);
            *(dest+3) = *(src+3);;
            *(dest+4) = '\0';
            break;
        }
        if ((buffer & 0x0000FF0000000000) == 0) {
            *dest = *src;
            *(dest+1) = *(src+1);
            *(dest+2) = *(src+2);
            *(dest+3) = *(src+3);;
            *(dest+4) = *(src+4);;
            *(dest+5) = '\0';
            break;
        }
        if ((buffer & 0x00FF000000000000) == 0) {
            *dest = *src;
            *(dest+1) = *(src+1);
            *(dest+2) = *(src+2);
            *(dest+3) = *(src+3);;
            *(dest+4) = *(src+4);;
            *(dest+5) = *(src+5);;
            *(dest+6) = '\0';
            break;
        }
        if ((buffer & 0xFF00000000000000) == 0) {
            *dest = *src;
            *(dest+1) = *(src+1);
            *(dest+2) = *(src+2);
            *(dest+3) = *(src+3);;
            *(dest+4) = *(src+4);;
            *(dest+5) = *(src+5);;
            *(dest+6) = *(src+6);;
            *(dest+7) = '\0';
            break;
        }
        *((unsigned long long int *)dest) = buffer;
        dest = dest + 8;
        src = src+8;
    }

    return tmp;
}
