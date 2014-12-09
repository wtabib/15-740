#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ELEM        10000
#define NUM_THREADS     50


pthread_mutex_t llock;
clock_t clock_arr[NUM_THREADS];
int *lock;
int a[NUM_ELEM];   


void foo (int* a, int* lock){
    return;
}

void* spawnThread(void * obj) {
    
    int *j = (int*) obj;
    std::cout << "st: " << *j << std::endl;
    clock_t t;
    t = clock();
    for(int i=0; i<NUM_ELEM; i++){
        foo(&a[i], lock);
    }
    t = clock() - t;


    clock_arr[*j] = t;
}


int main(){

    for (int i = 0; i < NUM_THREADS; i++) {
        clock_arr[i] = 0;
    }


    lock = (int *) malloc(sizeof( int));
    //int a[NUM_ELEM] ;

    for (int i = 0 ; i < NUM_ELEM; i++ ) {
        a[i] = 0;
    }

    pthread_t t[NUM_THREADS];

    int idx_arr[NUM_THREADS];
    for (int i = 0 ; i < NUM_THREADS; i++) {
        idx_arr[i] = i;
        pthread_create(&(t[i]), NULL, spawnThread, &idx_arr[i]);
    }   

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(t[i], NULL);
    }   

    for (int i = 0; i < NUM_ELEM ; i++) {
        //std::cout << a[i] << std::endl;
    }
    
    clock_t q = 0;
    for (int i = 0; i < NUM_THREADS; i++){
        q = q + clock_arr[i];

    }
    std::cout << q <<std::endl;
    return 0;
}
