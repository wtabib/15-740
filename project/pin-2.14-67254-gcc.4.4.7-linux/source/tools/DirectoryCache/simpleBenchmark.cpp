#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gettime.h"
#include "directoryCache.h"
#include <sys/resource.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

#define DIRECTORY_CACHE

pthread_mutex_t lock;
unsigned long long int totalTime = 0;
unsigned long long int startTime;
unsigned long long int stopTime;

int load(int i, int *readFromArr) {
    return *(readFromArr+i);
}

void store(int* writeToArr, int data, int i) {
    *(writeToArr+i)=data;
}

void* spawnThread(void *obj) {
    Arr *arr = (Arr *) obj;
    for(int i=0; i<NUM_ELEM; i++){

        pthread_mutex_lock(&lock);
#ifdef DIRECTORY_CACHE
        stopTime = rdtsc();
        int pid = (int) syscall(__NR_gettid);
#endif
        int data = load(i, arr->r);
#ifdef DIRECTORY_CACHE
        int cycle_counts_r = updateDirectoryRead(pid, arr->r+i);
#endif
        store(arr->w, data, i);
#ifdef DIRECTORY_CACHE
        int cycle_counts_w = updateDirectoryWrite(pid, arr->w+i);
        totalTime += (stopTime - startTime)+cycle_counts_r+cycle_counts_w;
        startTime = rdtsc();
#endif
        pthread_mutex_unlock(&lock);
    }
}


int main(){

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


    Arr arr;

    for (int i = 0 ; i < NUM_ELEM; i++ ) {
        arr.w[i] = i;
        arr.r[i] = 0;
    }

    std::cout << rdtsc() << std::endl;
    startTime = rdtsc();
    pthread_t t[NUM_THREADS];

    for (int i = 0 ; i < NUM_THREADS; i++) {
        pthread_create(&(t[i]), NULL, spawnThread, &arr);
    }   

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(t[i], NULL);
    }   

    std::cout << rdtsc()-startTime << std::endl;
    std::cout << totalTime << std::endl;
    return 0;
}
