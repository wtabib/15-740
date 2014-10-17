#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include "func_time.h"

long long int NUM_TIMES = 100000000000L;
int NUM_THREADS = 16;
pthread_mutex_t print_mutex;
int t = 1;

void * add(void* in) {
    long long int i = 0;
    int total = 0;

   // clock_t start = clock();
   // clock_t diff;
    for (i = 0; i < NUM_TIMES; i++ ) {
        total = total+i;
    }

    // diff = clock() - start;
    // int time = diff/CLOCKS_PER_SEC;

   // pthread_mutex_lock(&print_mutex);
   // printf("Time for thread %d\n", time);
   // pthread_mutex_unlock(&print_mutex);
    return NULL; 
}


void wrapper_func(){
    int i = 0;
    pthread_t threads[t];
    int inputs[NUM_THREADS];
    for(i= 0; i < t; i++){
        pthread_create(&(threads[i]), NULL, add, &(inputs[i]));
    }

    for (i = 0; i < t; i++) {
        pthread_join(threads[i], NULL);
    }
} 

int main() {

    int j = 0, k = 0;
    struct timespec start_t, end_t;

    for (j = 17; j > 15; j--) {
        t = j;
        printf("----------------- Number of threads: %d ------------------\n", j);
        clock_gettime(CLOCK_REALTIME, &start_t);
        for (k = 0; k < 10; k++) {
            wrapper_func();
        }
        clock_gettime(CLOCK_REALTIME, &end_t);
        printf("%g\n", ((double)((end_t.tv_sec-start_t.tv_sec)+(end_t.tv_nsec-start_t.tv_nsec)/1e9))/10);

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
