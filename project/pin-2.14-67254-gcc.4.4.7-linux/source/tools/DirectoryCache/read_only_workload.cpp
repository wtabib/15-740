#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gettime.h"
#include <sys/resource.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include "directoryCache.h"

#define DIRECTORY_CACHE
#define TESTANDSET
int BUS_TRANSFER = 0;
int WAIT_TIME = 0;
int LOOKUP = 0;
int FLUSH = 0;

pthread_mutex_t lock;
int *lockTS;
unsigned long long int totalTime = 0;
unsigned long long int startTime;
unsigned long long int stopTime;

void testAndSet(int *lockTS) {
    int result = 1;
    int var2 = 0;
    asm volatile( "enter_region: lock;"
            "xchg %0, %1;"
            : "=r"(result), "=m"(*lockTS)
            : "0"(result), "m"(*lockTS)
            : "memory");
    asm volatile("cmp %%rbx, %%rax" : "=b"(result) : "b" (result), "a" (var2));
    asm volatile("jne enter_region");
}

int load(int i, int *readFromArr) {
    return *(readFromArr+i);
}

void store(int* writeToArr, int data, int i) {
    *(writeToArr+i)=data;
}

void* spawnThread(void *obj) {
    Arr *arr = (Arr *) obj;
    for(int i=0; i<NUM_ELEM; i++){

#ifndef TESTANDSET
        pthread_mutex_lock(&lock);
#endif
#ifdef TESTANDSET
        testAndSet(lockTS);
#endif
#ifdef DIRECTORY_CACHE
        stopTime = rdtsc();
        int pid = (int) syscall(__NR_gettid);
#endif
        int data = load(i, arr->r);
#ifdef DIRECTORY_CACHE
        int cycle_counts_r = updateDirectoryRead(pid, arr->r+i, WAIT_TIME,
                BUS_TRANSFER, LOOKUP, FLUSH);
#endif
#ifdef DIRECTORY_CACHE
        totalTime += (stopTime - startTime)+cycle_counts_r;
        startTime = rdtsc();
#endif
#ifndef TESTANDSET
        pthread_mutex_unlock(&lock);
#endif
#ifdef TESTANDSET
        *lockTS = 0;
#endif
    }
}


int main(int argc, char *argv[]){

    //get the pid of the main thread
    /*int which = PRIO_PROCESS;
    id_t pid;
    int ret;
    pid = getpid();
    ret = getpriority(which, pid);
    std::cout << "priority = " << ret << std::endl;*/

    //you can also set the priority on the command line using sudo and 
    //sudo nice -n -20 ./simpleBenchmark.o
    /*int priority = -20;
    ret = setpriority(which, pid, priority);
    std::cout << "return code = " << ret << std::endl;*/

    if (argc > 1) {
        WAIT_TIME = atoi(argv[1]);
        BUS_TRANSFER = atoi(argv[2]);
        LOOKUP = atoi(argv[3]);
        FLUSH = atoi(argv[4]);
    }

    lockTS = (int *)malloc(sizeof(int));
    *lockTS = 0;

    Arr arr;

    for (int i = 0 ; i < NUM_ELEM; i++ ) {
        arr.w[i] = i;
        arr.r[i] = 0;
    }

    startTime = rdtsc();
    pthread_t t[NUM_THREADS];

    for (int i = 0 ; i < NUM_THREADS; i++) {
        pthread_create(&(t[i]), NULL, spawnThread, &arr);
    }   

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(t[i], NULL);
    }   

    std::cout << totalTime << std::endl;
    return 0;
}
