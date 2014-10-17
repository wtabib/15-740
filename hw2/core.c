#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include "func_time.h"

long long int NUM_TIMES = 1000L;
int NUM_THREADS = 16;
pthread_mutex_t print_mutex;
int t = 1;
struct timespec start[100];
struct timespec end[100];

void * add(void* in) {
    long long int i = 0;
    int total = 0;
    int idx = *((int *) in);

   // clock_t start = clock();
   // clock_t diff;
    clock_gettime(CLOCK_REALTIME, &(start[idx]));
    for (i = 0; i < NUM_TIMES; i++ ) {
        total = total+i;
    }
    clock_gettime(CLOCK_REALTIME, &(end[idx]));
    double fixed_time = ((double)((end[idx].tv_sec-start[idx].tv_sec)+(end[idx].tv_nsec-start[idx].tv_nsec)/1e9));


    // diff = clock() - start;
    // int time = diff/CLOCKS_PER_SEC;

    //pthread_mutex_lock(&print_mutex);
    //printf("thread %d: %g\n", idx, fixed_time);
    //pthread_mutex_unlock(&print_mutex);
    return NULL; 
}


double wrapper_func(){
    int i = 0;
    pthread_t threads[t];
    int inputs[t];
    for(i= 1; i < t; i++){
        inputs[i] = i;
        pthread_create(&(threads[i]), NULL, add, &(inputs[i]));
    }

    inputs[0] = 0;
    add(&inputs[0]);

    for (i = 1; i < t; i++) {
        pthread_join(threads[i], NULL);
    }

    struct timespec min = start[0];
    struct timespec max = end[0];


    for(i = 0; i < t; i++) {
        if(start[i].tv_sec < min.tv_sec || 
           (start[i].tv_sec == min.tv_sec && start[i].tv_nsec < min.tv_nsec)) {
           min = start[i];
        }
    } 

    for(i = 0; i < t; i++) {
        if(end[i].tv_sec > max.tv_sec || 
           (end[i].tv_sec == max.tv_sec && end[i].tv_nsec > max.tv_nsec)) {
           max = end[i];
        }
    } 

    double fixed_time = ((double)((max.tv_sec-min.tv_sec)+(max.tv_nsec-min.tv_nsec)/1e9));
    return fixed_time;

} 

int main() {

    int j = 0, k = 0;
    struct timespec start_t, end_t;
    struct timespec start_j, end_j;

    for (j = 32; j > 0; j--) {
        t = j;
        printf("----------------- Number of threads: %d ------------------\n", j);

        
        clock_gettime(CLOCK_REALTIME, &start_j);
        NUM_TIMES = 1;
        for (k = 0; k < 100; k++){
            //wrapper_func();              
        }
        clock_gettime(CLOCK_REALTIME, &end_j);

        double fixed_time = ((double)((end_j.tv_sec-start_j.tv_sec)+(end_j.tv_nsec-start_j.tv_nsec)/1e9))/100;



        clock_gettime(CLOCK_REALTIME, &start_t);
        NUM_TIMES = 1000000;
        double running_sum = 0;
        for (k = 0; k < 100; k++) {
            running_sum += wrapper_func();
        }
        clock_gettime(CLOCK_REALTIME, &end_t);
        double measured_time = ((double)((end_t.tv_sec-start_t.tv_sec)+(end_t.tv_nsec-start_t.tv_nsec)/1e9))/100;
        double diff_time = measured_time - fixed_time;

        printf("%g\n", running_sum / 100.0);
// printf("%g\t%g\t%g\n", fixed_time, measured_time, diff_time);

        // printf("fixed: %g\n", fixed_time);
        // printf("measured: %g\n", ((double)((end_t.tv_sec-start_t.tv_sec)+(end_t.tv_nsec-start_t.tv_nsec)/1e9))/100);
        // printf("diff: %g\n", ((double)((end_t.tv_sec-start_t.tv_sec)+(end_t.tv_nsec-start_t.tv_nsec)/1e9))/100 - fixed_time);

        //long double time = func_time(wrapper_func, 0.02);
        //printf("time = %Lg\n", time/t);

        /*
        clock_t diff;
        int e;
        int i = 0;

        int time;

        clock_t start = clock();
        for (i = 0; i < j; i++) {
            pthread_create(&(threads[i]), NULL, add, &(inputs[i]));
        }

        for (i = 0; i < j; i++) {
            pthread_join(threads[i], NULL);
        }

        diff = clock() - start;
        time = diff * 1000/CLOCKS_PER_SEC;
        //printf("Time for thread %d: %d\n", i, time);
//        for (i = 0; i < j; i++) {
//            printf("Time for thread %d: %d\n", i, inputs[i]);
//        }
//
*/
    }
    return 0;
}
