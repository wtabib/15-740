#include <pthread.h>
#include <iostream>


#define NUM_ELEM        10000


void foo (int* a){
    return;
}

void* spawnThread(void * obj) {

    int* a = (int*)obj;
    for(int i=0; i<NUM_ELEM; i++){
       // a[i]++;
        foo(&a[i]);
    }
}


#define NUM_THREADS     50
int main(){ 

    int a[NUM_ELEM];

    for (int i = 0 ; i < NUM_ELEM; i++ ) {
        a[i] = 0;
    }

    pthread_t t[NUM_THREADS];

    for (int i = 0 ; i < NUM_THREADS; i++) {
        pthread_create(&(t[i]), NULL, spawnThread, a);
    }   

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(t[i], NULL);
    }   

    for (int i = 0; i < NUM_ELEM ; i++) {
        std::cout << *(a+i) << std::endl;
    }

    return 0;
}
