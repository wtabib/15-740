#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ELEM        10000
#define NUM_THREADS     50

struct clock_struct {
    int i;
    int *a;
};
 
pthread_mutex_t llock;
clock_t clock_arr[NUM_THREADS];
int *lock;
int a[NUM_ELEM];   



void foo (int* a, int* lock){
    return;
}

void* spawnThread(void * obj) {
    
    clock_t t;
    t = clock();
    int *j = (int*) obj;
    //int* a = (int*)obj;
    //int* i = (int*)counter;
    for(int i=0; i<NUM_ELEM; i++){
        pthread_mutex_lock(&llock);
        //b[cs->i]++;
        a[i]++;
        //cs->a[cs->i]++;
        pthread_mutex_unlock(&llock);
        //std::cout << a[i] << std::endl;
        //foo(&a[i], lock);
        //std::cout << a[i] << std::endl;
    }
    clock_arr[*j] = clock() - t;
}


int main(){


    lock = (int *) malloc(sizeof( int));
    //int a[NUM_ELEM] ;

    for (int i = 0 ; i < NUM_ELEM; i++ ) {
        a[i] = 0;
    }

    pthread_t t[NUM_THREADS];

    for (int i = 0 ; i < NUM_THREADS; i++) {
        pthread_create(&(t[i]), NULL, spawnThread, &i);
    }   

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(t[i], NULL);
    }   

    for (int i = 0; i < NUM_ELEM ; i++) {
        std::cout << a[i] << std::endl;
    }
    
    clock_t q = 0;
    for (int i = 0; i < NUM_THREADS; i++){
        q = q + clock_arr[i];

    }
    std::cout << q <<std::endl;
    return 0;
}
