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
// is an upper bound on delta (and is probably exactly it usually

long double compute_resolution(timer_init_funct init, timer_elapsed_funct etime) {
    init();
    long double ts = etime();
    int elapsed_non_neg = 0;
    int n = 1;
    
    while(1) {
        int a = 0;
        int j;
        for (j = 0; j < n; j++) {
            a += j;
        }
        long double tc = etime();
        long double diff =  tc - ts;
        if(diff > 0) {
            return diff;
        }

        n += 100;
    }
}

void dummy_func() {
    int a = 0, j = 0;
    for (j = 0; j < 6400000; j++)
    {
        a += j;
    }
}

//typedef void (*test_funct)(void);
//

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

     
     printf("time_difference/n = %.10Lf\n", time_difference/n);
     
     return time_difference/n;
}

long double func_time_generic(timer_init_funct init, timer_elapsed_funct elapsed,
                              test_funct P, long double E) {

    printf("Error bound: %Lf\n", E);
    long double delta = compute_resolution(init, elapsed);
    printf("Delta: %Lf\n", delta);
    long double threshold = delta/E;
    printf("Threshold: %Lf\n", threshold);

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
        printf("n = %ld\n", n);
        printf("Taggregate = %Lf\n", Taggregate);
        if(Taggregate > threshold) {
            printf("Exceeded!");
            return (Taggregate / (long double) n);
        } 

        n *= 2;
    }
}


long double func_time(test_funct P, long double E) {

    long double result =  func_time_generic(init_etime, get_etime, P, E);
    printf("More precise print: %Le", result);
    return result;

/*

    int i = 0, k, a = 0;
    struct itimerval start, end;
    start.it_interval.tv_sec = 0;
    start.it_interval.tv_usec = 0;
    start.it_value.tv_sec = MAX_ETIME;
    start.it_value.tv_usec = 0;
    setitimer(ITIMER_VIRTUAL, &start, NULL);
    long double error;



    long double last_time = 0, this_time = 0, b = 0, total = 0;
    long double count = 0;
    int n = 1;
    long double time_difference;

    long double t_a;
       
    printf("Computing actual time...\n");
    t_a  = compute_actual_time(P);
    printf("Done\nStarting n\n");

    while(1){
        //printf("n = %d\n", n);

        getitimer(ITIMER_VIRTUAL, &start);
        //this_time = (long double) ((first_u.it_value.tv_sec - curr.it_value.tv_sec) + (first_u.it_value.tv_usec - curr.it_value.tv_usec)*1e-6);

        printf("n = %d\n", n);
        for (k = 0; k < n; k++) { 

            P(); 
            
        } 


        getitimer(ITIMER_VIRTUAL, &end);

        //printf("start time = %Lf\n", (long double) start.it_value.tv_sec);
        //printf("end time = %Lf\n", (long double) end.it_value.tv_sec); 
        //last_time = (long double) ((first_u.it_value.tv_sec - curr.it_value.tv_sec) + (first_u.it_value.tv_usec - curr.it_value.tv_usec)*1e-6); 
        time_difference = (long double) ((start.it_value.tv_sec - end.it_value.tv_sec) + (start.it_value.tv_usec - end.it_value.tv_usec)*1e-6);

        error = (time_difference - n*t_a)/(n*t_a);
        if (error < 0)
            error = error*-1;

        printf("time_difference = %.10Lf\t\tcomputed error = %.10Lf\n", time_difference, error);

        if (error < E){
            printf("T_m = %.10Lf\n", time_difference);
            //return time_difference;
            return time_difference/n;
        }
        else{
            n= n*2;
        }


    }
*/
}

/*int main () {

    long double E = 0.5;
    //double result = func_time(dummy_func, E);
    //printf("result = %g\n", result);

    long double result = compute_actual_time(dummy_func);
    printf("result = %Lf\n", result);
    return 0;
}*/
