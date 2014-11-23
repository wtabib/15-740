#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define ELEM 100

int arr[1000];

void store(int n, int i){
    arr[i] = n;
}


int load(int n){
    return arr[n];
}


void* spawnThread(void * obj) {
    //std::cout << "from the thread:" << getpid() << std::endl;
    int * idx = (int *)obj;
    int i = 0;
    int a = 0;

    for( i = *idx; i < *idx+100; i++) {
        store(i, i);
    }

    for( i = *idx; i < *idx+100; i++) {
        a = load(i);
    }
}


#define NUM_THREADS     10
int main() {

    pthread_t thread[NUM_THREADS];
    

    /*for( i = 0; i < 100; i++){
        store(i, i);
    }

    for( i = 0; i < 100; i++){
        a = load(i);
    }*/

    int startIndices[NUM_THREADS];

    for (int i = 0 ; i < NUM_THREADS; i++) {
        startIndices[i] = i*100;
        pthread_create(&(thread[i]), NULL, spawnThread, &(startIndices[i]));
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread[i], NULL);
    }

    return 1;
}
