#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include "func_time.h"
#include <stdlib.h>

int NUM_THREADS = 32;
unsigned long long NUM_ELEM = 1000000L;
pthread_mutex_t print_mutex;
int t = 1;
struct timespec start[100];
struct timespec end[100];
int *arr;

void * add(void* in) {
    long long int i = 0;
    long long int j = 0;
    int total = 0;
    int idx = *((int *) in);

    int offset = idx*NUM_ELEM;
   // clock_t start = clock();
   // clock_t diff;
    clock_gettime(CLOCK_REALTIME, &(start[idx]));
    int k = 0;
    for (i = 0; i < NUM_ELEM; i+=64 ) {
        j = 10;
        k += j*j; 
        k += j*j; 
        k += j*j; 
        k += j*j; 
        k += j*j; 
        k += j*j; 
        k += j*j; 
        k += j*j; 
        k += j*j; 
        k += j*j; 
        k += j*j; 
        k += j*j; 
        k += j*j; 
        k += j*j; 
        total = total+arr[offset+i];
        total += k;
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
    arr = (int *) calloc(NUM_ELEM*NUM_THREADS, sizeof(int));

    for (j = 1; j < NUM_THREADS; j++) {
        t = j;
        
        clock_gettime(CLOCK_REALTIME, &start_t);
        double running_sum = 0;
        for (k = 0; k < 100; k++) {
            running_sum += wrapper_func();
        }
        clock_gettime(CLOCK_REALTIME, &end_t);

        printf("%d, %g\n", j, running_sum / 100.0);
    }
    return 0;
}
