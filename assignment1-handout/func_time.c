#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include "perf.h"
#include "func_time.h"

typedef void (*timer_init_funct)(void);
typedef long double (*timer_elapsed_funct)(void);

// Estimates the resolution, delta of a timer. Initializes timer, does dummy
// operations till it notices elapsed time is non zero and returns this. This
// is an upper bound on delta.

long double compute_resolution(timer_init_funct init, timer_elapsed_funct etime) {
    init();
    int n = 1;

    long double ts = etime();
    while(1) {
        long double tc = etime();
        long double diff =  tc - ts;
        if(diff > 0) {
            //printf("n = %d\n", n);
            return diff;
        }

        int a = 0;
        int j;
        for (j = 0; j < n; j++) {
            a += j;
        }
        n *= 2;
    }
}

long double compute_actual_time(test_funct P){
     struct itimerval start, end;
     start.it_interval.tv_sec = 0;
     start.it_interval.tv_usec = 0;
     start.it_value.tv_sec = MAX_ETIME;
     start.it_value.tv_usec = 0;
     setitimer(ITIMER_VIRTUAL, &start, NULL);
     int k;
     long double time_difference = 0;
   

     //this_time = (long double) ((first_u.it_value.tv_sec - curr.it_value.tv_sec) + (first_u.it_value.tv_usec - curr.it_value.tv_usec)*1e-6);

     int n = 1;
     while (1) {
         
        getitimer(ITIMER_VIRTUAL, &start);
        for (k = 0; k < n; k++) { 
            //printf("k = %d\n", k);
            P(); 
        } 
        getitimer(ITIMER_VIRTUAL, &end);
        time_difference = time_difference + (long double) ((start.it_value.tv_sec - end.it_value.tv_sec) + (start.it_value.tv_usec - end.it_value.tv_usec)*1e-6);

        if (time_difference > 0)  {
            n = n*1000;
            time_difference = 0;
            getitimer(ITIMER_VIRTUAL, &start);
            for (k = 0; k < n; k++) { 
                //printf("k = %d\n", k);
                P(); 
            } 
            getitimer(ITIMER_VIRTUAL, &end);
            time_difference = time_difference + (long double) ((start.it_value.tv_sec - end.it_value.tv_sec) + (start.it_value.tv_usec - end.it_value.tv_usec)*1e-6);
            break;
        }
        else {
            n = n*2;
        }
     }

     
     //printf("time_difference/n = %.10Lf\n", time_difference/n);
     
     return time_difference/n;
}

long double func_time_generic(timer_init_funct init, timer_elapsed_funct elapsed,
                              test_funct P, long double E) {

    //long double delta = compute_resolution(init, elapsed);
    long double delta = 4e-3;
    long double threshold = 2.0*delta/E + delta;

  
    //printf("Error bound: %Lf\n", E);
    //printf("Delta: %Le\n", delta);
    //printf("Threshold: %Lf\n", threshold);

    long int n = 1;
    while(1) {

        // Pseudo-code from the assignment
        init();
        long double Ts = elapsed();
        int i = 0;
        for(i = 0; i < n; i++) {
            P();
        }
        long double Tf = elapsed();
        long double Taggregate = Tf - Ts;
        //printf("n = %ld\n", n);
        //printf("Taggregate = %Lf\n", Taggregate);
        if(Taggregate > threshold) {
            //printf("Exceeded!");
            return (Taggregate / (long double) n);
        } 

        n *= 2;
    }
}


long double func_time(test_funct P, long double E) {
    long double result = func_time_generic(init_etime, get_etime, P, E);
    return result;
}

long double func_time_hw(test_funct P, long double E) {
    struct timespec res;
    clock_getres(4, &res);
    //printf("Real res for hw timer: %ld\n", res.tv_nsec);
    long double result = func_time_generic(init_etime_hw, get_etime_hw, P, E);
    return result;
}

