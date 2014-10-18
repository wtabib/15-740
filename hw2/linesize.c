#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include "func_time.h"
#include <features.h>
#include <sched.h>
#include <stdlib.h>


#define NUM_ITER 256
#define NUM_TIMES 10000
#define NUM_ELEM 4096
volatile int *list;

void * add(void* in) {
    int idx = *((int *) in);
    cpu_set_t cpuset1;
    CPU_ZERO(&cpuset1);
    if (idx == 0){
        CPU_SET(0, &cpuset1);
    }else{
        CPU_SET(4, &cpuset1);
    }
    pthread_t thread1 = pthread_self();
    pthread_setaffinity_np(thread1, sizeof(cpu_set_t), &cpuset1);


    /*cpu_set_t cpuset1Get;
    pthread_getaffinity_np(thread1, sizeof(cpu_set_t), &cpuset1Get);
    if (idx == 0 && CPU_ISSET(0, &cpuset1Get) == 0) {
        printf("Failure!");
        return NULL;
    }
    if (idx && CPU_ISSET(4, &cpuset1Get) == 0) {
        printf("Failure!");
        return NULL;
    }*/

    long long int i = 0;

    for (i = 0; i < NUM_TIMES; i++ ) {
        list[idx] = list[idx]+i;
    }
    return NULL; 
}



int main() {

    list = (int *)calloc(NUM_ELEM, sizeof(int));

    cpu_set_t cpusetMain;
    CPU_SET(7, &cpusetMain);
    pthread_t mainThread = pthread_self();
    pthread_setaffinity_np(mainThread, sizeof(cpu_set_t), &cpusetMain);

    cpu_set_t cpusetMainGet;
    pthread_getaffinity_np(mainThread, sizeof(cpu_set_t), &cpusetMainGet);
    //printf("main was assigned to cpu 7? %d\n", CPU_ISSET(7, &cpusetMainGet));


    pthread_t thread1, thread2;
    int val1=0, val2, i = 0;
    struct timespec start_t, end_t;
    struct timespec start_j, end_j;

    unsigned long listaddr = (unsigned long) &list;
    unsigned long offset = (2048 - (listaddr % 2048))/4;
    //printf("listaddr = %lu\n\t\toffset = %lu\t\t&list[offset]=%p\n", listaddr, offset, &list[offset]);

    for (i = 1; i < NUM_ITER; i++) {
         
        val2 = i+offset;
        val1 = offset;
        list[val1] = 0;
        list[val2] = 0;

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
