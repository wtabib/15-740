#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include "perf.h"
#include "func_time.h"


void dummy_func() {
    int a = 0, j = 0;
    for (j = 0; j < 6400000; j++)
    {
        a += j;
    }
}

//typedef void (*test_funct)(void);

long double func_time(test_funct P, long double E) {


    //struct itimerval start, end; /* user time */
    int i = 0, k, a = 0;
    struct itimerval start, end;
    start.it_interval.tv_sec = 0;
      start.it_interval.tv_usec = 0;
      start.it_value.tv_sec = MAX_ETIME;
      start.it_value.tv_usec = 0;
      setitimer(ITIMER_VIRTUAL, &start, NULL);

      /*end.it_interval.tv_sec = 0;
      end.it_interval.tv_usec = 0;
      end.it_value.tv_sec = MAX_ETIME;
      end.it_value.tv_usec = 0;
      setitimer(ITIMER_VIRTUAL, &end, NULL);*/



    long double last_time = 0, this_time = 0, b = 0, total = 0;
    long double count = 0;
    int n = 1;
    long double time_difference;
    while(1){
        //printf("n = %d\n", n);

        getitimer(ITIMER_VIRTUAL, &start);
        //this_time = (long double) ((first_u.it_value.tv_sec - curr.it_value.tv_sec) + (first_u.it_value.tv_usec - curr.it_value.tv_usec)*1e-6);

        for (k = 0; k < n; k++) { 

            P(); 
            /*int a = 0, j = 0;
            for (j = 0; j < 10000; j++)
            {
                a += j;
            }*/

        } 

        getitimer(ITIMER_VIRTUAL, &end);

        //printf("start time = %Lf\n", (long double) start.it_value.tv_sec);
        //printf("end time = %Lf\n", (long double) end.it_value.tv_sec); 
        //last_time = (long double) ((first_u.it_value.tv_sec - curr.it_value.tv_sec) + (first_u.it_value.tv_usec - curr.it_value.tv_usec)*1e-6); 
        time_difference = (long double) ((start.it_value.tv_sec - end.it_value.tv_sec) + (start.it_value.tv_usec - end.it_value.tv_usec)*1e-6);

        n = n*2;

        if (time_difference > 0){
            printf("time difference = %Lf\n", time_difference);
            return time_difference;
        }
    }
}

int main () {

    long double E = 4;
    double result = func_time(dummy_func, E);
    printf("result = %g\n", result);
    return 0;
}
