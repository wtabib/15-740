#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include "func_time.h"
#include <features.h>
#include <sched.h>


#define NUM_ITER 256
#define NUM_TIMES 10000
volatile char list [4096];

void * add(void* in) {
    int idx = *((int *) in);
    cpu_set_t cpuset1;
    CPU_ZERO(&cpuset1);
    if (idx == 0){
        CPU_SET(0, &cpuset1);
    }else{
        CPU_SET(1, &cpuset1);
    }
    pthread_t thread1 = pthread_self();
    pthread_setaffinity_np(thread1, sizeof(cpu_set_t), &cpuset1);

    long long int i = 0;

    for (i = 0; i < NUM_TIMES; i++ ) {
        list[idx] = list[idx]+1;
    }
    return NULL; 
}



int main() {

    pthread_t thread1, thread2;
    int val1=0, val2, i = 0;
    struct timespec start_t, end_t;
    struct timespec start_j, end_j;

    unsigned int listaddr = &list;
    unsigned int offset = 2048 - (listaddr % 2048);

    for (i = 1; i < NUM_ITER; i++) {
         
        val2 = i+offset;
        val1 = offset;
        list[val1] = 'a';
        list[val2] = 'a';

        clock_gettime(CLOCK_REALTIME, &start_j);
        pthread_create(&(thread1), NULL, add, &(val1));
        pthread_create(&(thread2), NULL, add, &(val2));
            
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        clock_gettime(CLOCK_REALTIME, &end_j);

        double fixed_time = ((double)((end_j.tv_sec-start_j.tv_sec)+(end_j.tv_nsec-start_j.tv_nsec)/1e9));
        printf("%d,%g\n", i, fixed_time);
    }

    return 0;
}
