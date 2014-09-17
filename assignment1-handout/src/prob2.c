#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include "../perf.h"



int main(int argc, char **argv) {

    struct itimerval start, end; /* user time */
    int i = 0, j, a = 0;
    start.it_interval.tv_sec = 0;
    start.it_interval.tv_usec = 0;
    start.it_value.tv_sec = MAX_ETIME;
    start.it_value.tv_usec = 0;
    setitimer(ITIMER_VIRTUAL, &start, NULL);

    //struct itimerval start, end;
    long double this_time = 0, b = 0, total = 0;
    long double count = 0;
    int n = 1;
    //for (i = 0; i < 1; i++) {
    while (1) {
        getitimer(ITIMER_VIRTUAL, &start);
        for (j = 0; j < n; j++)
        {
            a += j;
        }
        getitimer(ITIMER_VIRTUAL, &end);
        this_time = (long double) ((start.it_value.tv_sec - end.it_value.tv_sec) + (start.it_value.tv_usec - end.it_value.tv_usec)*1e-6);
        printf("this time = %Lf\n", this_time);
        total = total + this_time;
        if(this_time > 0){
            printf ("n = %d\t\t%Lf\n", n, this_time);
            count++;
            break;
        }
        n = n+100;
    }
    

    printf("delta = %Lf\n", total/count);

    return 0;
}

