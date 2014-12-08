#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define NUM_ELEM        10000
unsigned int *lock;

static inline unsigned long tas(volatile unsigned int* ptr)
{
    unsigned long result = 15;
    std::cout << "before" << std::endl;
    std::cout << "\tresult = " << result << ", ptr = " << *ptr << std::endl;
    asm volatile("lock;"
            "xchg %0, %1;"
            : "=r"(result), "=m"(*ptr)
            : "0"(result), "m"(*ptr)
            : "memory");

    std::cout << "after" << std::endl;
    std::cout << "\tresult = " << result << ", ptr = " << *ptr << std::endl;
    return result;
}

void* spawnThread(void * obj) {
    //asm("enter region: tsl reg, flag");

    int result = 1;
    int var2 = 0;

    //asm("enter_region: mov %1, %0" : "=r" (resul : "r" (var1));
    asm volatile( "enter_region: lock;"
            "xchg %0, %1;"
            : "=r"(result), "=m"(*lock)
            : "0"(result), "m"(*lock)
            : "memory");
    asm volatile("cmp %%rbx, %%rax" : "=b"(result) : "b" (result), "a" (var2));
    asm volatile("jne enter_region");

    //asm("enter_region: tsl %0, %1" : "=r" (var1) : "r" (lock) );
    //asm("cmp %0, #0" : "=r" (lock));
    int* a = (int*)obj;
    for(int i=0; i<NUM_ELEM; i++){
       a[i]++;
        //foo(&a[i]);
    }
    asm volatile("mov %0, %1" : "=r" (*lock) : "r" (var2));

}


#define NUM_THREADS     50
int main(){ 
    lock = (unsigned int *) malloc(sizeof(unsigned int));
    *lock = 0;
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
