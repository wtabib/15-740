/* Compilation:

   gcc -Wall -o pthread_clock_test.o -c pthread_clock_test.c
   gcc -Wall -o pthread_clock_test pthread_clock_test.o -lrt -lpthread
   */

#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>

pid_t wojtek_get_tid(){
    pid_t tid;
    tid = (long) syscall(SYS_gettid);
    return tid;
}

long long int NUM_TIMES = 10000000000L;
void * add(void* buf) {
    long long int i = 0;
    long long int total = 0;
    volatile long long sink;
    for (i = 0; i < NUM_TIMES; i++ ) {
        total = total+i;
    }
    sink = total;
    return NULL; 
}

#include <time.h>
void* test_clock_gettime(void* buf){
    struct timespec start_timespec,end_timespec;
    double start_nanosec,end_nanosec;
    int res;
    //printf("can measure thread cpu time:%ld", sysconf(_POSIX_THREAD_CPUTIME));

    res = clock_gettime(CLOCK_THREAD_CPUTIME_ID,&start_timespec);
    start_nanosec = (double)(start_timespec.tv_sec)*1e9 + (double)(start_timespec.tv_nsec);

    //add();
    res = clock_gettime(CLOCK_THREAD_CPUTIME_ID,&end_timespec);
    end_nanosec = (double)(end_timespec.tv_sec)*1e9 + (double)(end_timespec.tv_nsec);

    fprintf(stderr,"%d:%d time: %g s\n",getpid(),wojtek_get_tid(),(double)(end_nanosec-start_nanosec)/1e9);
    return 0;
}

#include <sys/time.h>
#include <sys/resource.h>
#define NUM_THREADS     32

int main(int argc, char** argv){
    char *message1 = "Thread 1";
    //char *message2 = "Thread 2";
    //int iret1, iret2;
    int  iret[NUM_THREADS];
    pthread_t threads[NUM_THREADS];
    struct timeval start, end;
    //pthread_t thread1, thread2;

    //  printf("can use per thread cpu timer: %d\n",sysconf(_POSIX_THREAD_CPUTIME)!=0);
    //  printf("using getrusage:\n");
    //  iret1 = pthread_create( &thread1, NULL, test_getrusage, (void*) message1);
    //  iret2 = pthread_create( &thread2, NULL, test_getrusage, (void*) message2);
    //  pthread_join( thread1, NULL);
    //  pthread_join( thread2, NULL); 
    int i, j;
    for (j = 1; j < NUM_THREADS; j++) {
        printf("----------------- Number of threads: %d ------------------\n", j);
        struct timespec start_timespec,end_timespec;
        double start_nanosec,end_nanosec;
        int res, time;
        clock_t diff;//, start, end;
        //printf("can measure thread cpu time:%ld", sysconf(_POSIX_THREAD_CPUTIME));

        //res = clock_gettime(CLOCK_THREAD_CPUTIME_ID,&start_timespec);
        //start_nanosec = (double)(start_timespec.tv_sec)*1e9 + (double)(start_timespec.tv_nsec);
        //start = clock();
        //printf("start: %g\n", (double)start);
        gettimeofday(&start, 0);
        for (i = 0; i < j; i++) {
            iret[i] = pthread_create( &(threads[i]), NULL, add, (void*) message1);
        }

        for (i = 0; i < j; i++) {
            pthread_join(threads[i], NULL);
        }
        gettimeofday(&end, 0);

        unsigned long long secs = (unsigned long long) ((end.tv_sec - start.tv_sec)*1000);
        unsigned long long millis = (unsigned long long ) ((end.tv_usec - start.tv_usec)/1000);
        unsigned long long total = secs + millis;
        printf("elapsed time: %llu\n", total);

        //end = clock();
        //printf("end: %g\n", (double)end);
        //add();
        //diff = end - start;

        //time = diff * 1000/CLOCKS_PER_SEC;
        //printf("time (clock): %d\n", time);
        //res = clock_gettime(CLOCK_THREAD_CPUTIME_ID,&end_timespec);
        //end_nanosec = (double)(end_timespec.tv_sec)*1e9 + (double)(end_timespec.tv_nsec);

        //printf(stderr,"%d:%d time: %g s\n",getpid(),wojtek_get_tid(),(double)(end_nanosec-start_nanosec)/1e9);
 
        

        /*for(i=1; i<=16; i++){
            int k;
            for (k=0; k<100; k++){
                printf("K: %d\n", k);
                printf("using clock_gettime:\n");
                iret1 = pthread_create( &thread1, NULL, test_clock_gettime, (void*) message1);
                iret2 = pthread_create( &thread2, NULL, test_clock_gettime, (void*) message2);
                pthread_join( thread1, NULL);
                pthread_join( thread2, NULL);
            }
            return 0;
        }*/
    }

    return 0;
}

