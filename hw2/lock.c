#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include "func_time.h"
#include "math.h"
#include <assert.h>
#include <semaphore.h>

#define MAX_THREADS     17
#define MAX_ITER        10 

int num_threads = 1;
int mode = 0;
int memloc = 0;

sem_t mySem;

struct timespec start[16];
struct timespec end[16];
void atomicIncr(int * location) {
    asm("lock incl (%0)" : : "r" (location) : "0");
}

void atomicIncrSem(int * location) {
    sem_wait(&mySem);
    memloc += 1;
    sem_post(&mySem);
}

void * add(void* in) {

    //looks up value of num_threads
    int idx = *((int *) in);
    int num_adds = pow(2,20)/num_threads;
    int i = 0;

    clock_gettime(CLOCK_REALTIME, &(start[idx]));
    if (!mode) {
        for (i = 0; i < num_adds; i++) {
            atomicIncr(&memloc);

        }
    }
    else {
        for (i = 0; i < num_adds; i++) {
            atomicIncrSem(&memloc);
        }
    }

    clock_gettime(CLOCK_REALTIME, &(end[idx]));

    return NULL; 
}


double wrapper_func(){
    memloc = 0;
    int i = 0;
    pthread_t threads[num_threads];
    int inputs[num_threads];

    sem_init(&mySem, 0, 1);

    for(i= 1; i < num_threads; i++){
        inputs[i] = i;
        pthread_create(&(threads[i]), NULL, add, &(inputs[i]));
    }

    inputs[0] = 0;
    add(&inputs[0]);

    for (i = 1; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    struct timespec min = start[0];
    struct timespec max = end[0];


    for(i = 0; i < num_threads; i++) {
        if(start[i].tv_sec < min.tv_sec || 
                (start[i].tv_sec == min.tv_sec && start[i].tv_nsec < min.tv_nsec)) {
            min = start[i];
        }
    } 

    for(i = 0; i < num_threads; i++) {
        if(end[i].tv_sec > max.tv_sec || 
                (end[i].tv_sec == max.tv_sec && end[i].tv_nsec > max.tv_nsec)) {
            max = end[i];
        }
    } 

    double fixed_time = ((double)((max.tv_sec-min.tv_sec)+(max.tv_nsec-min.tv_nsec)/1e9));

    __sync_synchronize();
    //printf("num_threads = %d, pow(2,20)/num_threads*num_threads = %d, memloc = %d\n", 
     //       num_threads, (int) (pow(2,20)/num_threads)*num_threads, memloc);
    //assert(memloc == (int) (pow(2,20)/num_threads)*num_threads);
    return fixed_time;

} 


int main() {

    int j = 0, k = 0;
    struct timespec start_t, end_t;
    struct timespec start_j, end_j;

    for (j = 1; j < MAX_THREADS; j++) {
        num_threads = j;
        
        clock_gettime(CLOCK_REALTIME, &start_t);
        double running_sum = 0;
        mode = 0;
        for (k = 0; k < MAX_ITER; k++) {
            running_sum += wrapper_func();
        }
        clock_gettime(CLOCK_REALTIME, &end_t);

        printf("atomic increment: %d, %g\n", j, running_sum / (double) MAX_ITER);
    }

    for (j = 1; j < MAX_THREADS; j++) {
        num_threads = j;
        
        clock_gettime(CLOCK_REALTIME, &start_t);
        double running_sum = 0;
        mode = 1;
        for (k = 0; k < MAX_ITER; k++) {
            running_sum += wrapper_func();
        }
        clock_gettime(CLOCK_REALTIME, &end_t);

        printf("semaphore increment: %d, %g\n", j, running_sum / (double) MAX_ITER );
    }

    return 0;
}
