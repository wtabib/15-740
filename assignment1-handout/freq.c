/*
 * Estimate the clock frequency
 */ 
#include <stdio.h>
#include "func_time.h"

int ax = 1;
int bx = 1;

/* Compute peformance of CPU doing repeated additions */
void add_test(void)
{
    int a = ax;
    int b = bx;

    asm ( " addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; "
        " addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; "
        " addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; "
        " addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; "
        " addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; "
        " addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; "
        " addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; "
        " addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; "
        " addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; "
        " addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; "
        " addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; "
        " addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; "
        " addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; "
        " addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; "
        " addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; "
        " addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; "
        " addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; "
        " addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; "
        " addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; "
        " addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; addl %2, %1; "
        " mov %1, %0; "
        :"=r"(ax)
        :"r"(a), "r"(b)
        :"%eax"
            );
}

void add_dummy(void)
{
    int a = ax;
    int b = bx;
    ax = a + b;
}

int main(int argc, char** argv)
{
    long double mhz;
    long double secs;

    /* Do this twice.    The first time warms up the cache: */
    secs = func_time(add_test, 0.02) - func_time(add_dummy, 0.02);
    secs = func_time(add_test, 0.02) - func_time(add_dummy, 0.02);

    mhz = (100.0/secs)/1000000.0;
    printf("The clock frequency is approximately %.0Lf Megahertz\n", mhz);
}
