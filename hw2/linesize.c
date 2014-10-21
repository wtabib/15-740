#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include "func_time.h"
#include <features.h>
#include <sched.h>
#include <stdlib.h>
#include "perf.h"


// Atomic = 0, no synch
// Atomic = 1, fetch_and_add (on x86 looks lock LOCK add)
// Atomic = 2, regular add followed by mfence
#define ATOMIC   2
#define NUM_ITER 256
#define NUM_TIMES 10000
#define NUM_ELEM 4096
volatile char *list;
unsigned long offsets[2];

void * add(void* in) {
    int idx = *((int *) in);
    unsigned long offset = offsets[idx];

    cpu_set_t cpuset1;
    CPU_ZERO(&cpuset1);
    if (idx == 0){
        //printf("Set to 0, %lu\n", offset);
        CPU_SET(0, &cpuset1);
    }else{
        //printf("Set to 1, %lu\n", offset);
        CPU_SET(1, &cpuset1);
    }

    pthread_t thread1 = pthread_self();
    pthread_setaffinity_np(thread1, sizeof(cpu_set_t), &cpuset1);

    long long int i = 0;

    for (i = 0; i < NUM_TIMES; i++ ) {

        #if ATOMIC == 0
          list[offset] = list[offset]+1;
        #elif ATOMIC == 1
          __sync_fetch_and_add(&list[offset], 1);
        #else 
          list[offset] = list[offset]+1;
          __sync_synchronize();
        #endif
    }
    return NULL; 
}



int main() {

    list = (char *)calloc(NUM_ELEM, sizeof(char));

    cpu_set_t cpusetMain;
    CPU_SET(7, &cpusetMain);
    pthread_t mainThread = pthread_self();
    pthread_setaffinity_np(mainThread, sizeof(cpu_set_t), &cpusetMain);

    cpu_set_t cpusetMainGet;
    pthread_getaffinity_np(mainThread, sizeof(cpu_set_t), &cpusetMainGet);


    pthread_t thread1, thread2;
    int idxs[2];
    idxs[0] = 0;
    idxs[1] = 1; 
    int val1=0, val2, i = 0;
    struct timespec start_t, end_t;
    struct timespec start_j, end_j;

    unsigned long listaddr = (unsigned long) &list;
    unsigned long offset = (2048 - (listaddr % 2048));
    int j = 0;

    double totalTimes[NUM_ITER];
    for (j = 0; j <= NUM_ITER; j++) {
        totalTimes[j] = 0;
    }

    unsigned long long totalMisses[NUM_ITER];
    for (j = 0; j <= NUM_ITER; j++) {
        totalMisses[j] = 0;
    }

    for (j = 0; j < 100; j++) {
        for (i = 1; i <= NUM_ITER; i++) {

            double total = 0.0;

            offsets[0] = i+offset;
            offsets[1] = offset;
            list[offsets[0]] = 0;
            list[offsets[1]] = 0;

            clock_gettime(CLOCK_REALTIME, &start_j);
            pthread_create(&(thread1), NULL, add, &idxs[0]);
            pthread_create(&(thread2), NULL, add, &idxs[1]);

            pthread_join(thread1, NULL);
            pthread_join(thread2, NULL);
            clock_gettime(CLOCK_REALTIME, &end_j);

            double fixed_time = ((double)((end_j.tv_sec-start_j.tv_sec)
                                  +(end_j.tv_nsec-start_j.tv_nsec)/1e9));
            totalTimes[i] = totalTimes[i]+fixed_time;
        }
    }

    for (i = 1; i <= NUM_ITER; i++) {
        printf("%lu,%g\n", i * sizeof(char), totalTimes[i]/100);
    }

    return 0;
}
