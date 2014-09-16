#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include "../perf.h"



int main(int argc, char **argv) {

    struct itimerval first_u; /* user time */
    int i = 0, j, a = 0;
    first_u.it_interval.tv_sec = 0;
    first_u.it_interval.tv_usec = 0;
    first_u.it_value.tv_sec = MAX_ETIME;
    first_u.it_value.tv_usec = 0;
    setitimer(ITIMER_VIRTUAL, &first_u, NULL);

    struct itimerval curr;
    long double last_time = 0, this_time = 0, b = 0, total = 0;
    long double count = 0;
    for (i = 0; i < 1000; i++) {
        for (j = 0; j < 10000; j++)
        {
            a += j;
        }
        getitimer(ITIMER_VIRTUAL, &curr);
        this_time = (long double) ((first_u.it_value.tv_sec - curr.it_value.tv_sec) + (first_u.it_value.tv_usec - curr.it_value.tv_usec)*1e-6);
        if (i == 0)
            last_time = this_time;
        else if (last_time != this_time) {
            b = this_time - last_time;
            total = total + b;

            count++;
        }
        last_time = this_time;

    }

    printf("delta = %Lf\n", total/count);

    return 0;
}

