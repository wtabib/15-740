/* 
simple example of how to use the 
interval timer from the perf package
*/

#include <stdio.h>
#include "perf.h"

main(int argc, char **argv) 
{
    int i;
    int a = 0;
    double start_time, end_time, secs;

    /* call init_etime to initialize the timer value */
    init_etime();

    start_time = get_etime();
    for (i = 0; i < 1e9; i++)
    {
        a += i;
    }
    end_time = get_etime();
    secs = end_time - start_time;

    printf("The loop computed %d and took %f secs\n", a, secs);
}
