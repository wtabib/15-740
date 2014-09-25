#include <stdio.h>
#include "strcat_opt.h"

char *strcat_opt(char *dest, const char *src) {

    unsigned long long int buffer;
    char *tmp = dest;

    while (1) {
        buffer = *((unsigned long long int *) dest);
        //printf("%llx\n", buffer);
        //printf("buffer & 0xFF == %llx\n", buffer & 0x00000000000000FF);
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

    for (;;) {
        *dest = *src;
        if (*src == '\0') {
            break;
        }
        src++;
        dest++;
    }

    return tmp;
}
