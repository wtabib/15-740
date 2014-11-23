#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define ELEM 100

int arr[100];

void store(int n, int i){
    arr[i] = n;
}


int load(int n){
    return arr[n];
}


void* spawnThread(void *) {
    //std::cout << "from the thread:" << getpid() << std::endl;
    int i = 0;
    int a = 0;

    for( i = 0; i < 100; i++) {
        store(i, i);
        a = load(i);
    }

    for( i = 0; i < 100; i++) {
        a = load(i);
    }
}


int main() {

    //std::cout << "from the main:" << getpid() << std::endl;
    //int i;
    //int a;
    int arr[100];

    pthread_t thread[2];
    

    /*for( i = 0; i < 100; i++){
        store(i, i);
    }

    for( i = 0; i < 100; i++){
        a = load(i);
    }*/

    pthread_create(&(thread[0]), NULL, spawnThread, NULL);
    pthread_create(&(thread[1]), NULL, spawnThread, NULL);

    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);

    return 1;
}
