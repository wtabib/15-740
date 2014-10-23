#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include "func_time.h"
#include <stdlib.h>
#include "math.h"
#include <assert.h>

#define THREADS 6
#define BLOCK  16

//int THREADS = 4;
//int BLOCK = 4;

pthread_mutex_t print_mutex;
int *arr;
// Made global because lazy
int *a;
int *b;
int *c;
int dim = 0;

int min(int a, int b) {
    if (a < b)
        return a;
    else
        return b;
}

int max(int a, int b) {
    if (a > b)
        return a;
    else
        return b;
}

void serialMatrixMultiply(int *a, int *b, int *c, int dim) {

    int i = 0, j = 0, jj = 0, kk = 0, k = 0;
    double sum = 0.0;

    //based on code in slide 61 of 03-memhier.pdf lecture slides
    for (jj = 0; jj < dim; jj += BLOCK) {
        for (i = 0; i < dim; i++) {
            for (j=jj; j < min(jj+BLOCK,dim); j++)
                c[i*dim+j] = 0.0;
        }
        for (kk = 0; kk < dim; kk+=BLOCK) {
            for (i = 0; i < dim; i++) {
                for (j = jj; j < min(jj+BLOCK,dim); j++) {
                    sum = 0.0;

                    for (k = kk; k < min(kk+BLOCK, dim); k++) {
                        sum += a[i*dim+k] * b[k*dim+j];
                    }
                    c[i*dim+j] += sum;
                }
            }
        }
    }
}

void * parallelMatrixMultiply_thread(void* in) {

    int idx = *((int *) in);
    int i = 0, j = 0, jj = 0, kk = 0, k = 0;
    double sum = 0.0;
    int l = 0;

    int num_blocks_thread = pow(dim/BLOCK,2)/THREADS;
    //printf("dim = %d, BLOCK = %d, THREADS = %d\n", dim, BLOCK, THREADS);
    //printf("num_blocks_thread*THREADS = %d, pow(dim/BLOCK,2) == %g\n", num_blocks_thread*THREADS, pow(dim/BLOCK,2));
    assert(num_blocks_thread*THREADS == pow(dim/BLOCK,2));
    assert((dim/BLOCK)*BLOCK == dim);

    for (l= 0; l < num_blocks_thread; l++){
        int bidx = idx*num_blocks_thread + l;
        int blockcolumn = bidx % (dim / BLOCK);
        int blockrow = (bidx - blockcolumn) / (dim / BLOCK);
        assert(blockrow * (dim / BLOCK) + blockcolumn == bidx);



        jj = idx * BLOCK;

        int topleft = blockrow * (BLOCK * dim) + blockcolumn * BLOCK;

        for(i = 0; i < BLOCK; i++) {
            for(j = 0; j < BLOCK; j++) {
                c[topleft + i * dim + j] = 0.0;
            }
        }

        for(k = 0; k < dim / BLOCK; k++) {
            int toplefta = blockrow * (BLOCK * dim) + k * BLOCK;
            int topleftb = k * (BLOCK * dim) + blockcolumn * BLOCK;
            for(i = 0; i < BLOCK; i++) {
                for(j = 0; j < BLOCK; j++) {
                    sum = 0.0;
                    for(kk = 0; kk < BLOCK; kk++) { 
                        sum += a[toplefta + i * dim + kk] * b[topleftb + kk * dim + j];
                    }
                    c[topleft + i * dim + j] += sum;
                }
            }

        }
    }

    //    for (kk = 0; kk < dim; kk+=BLOCK) {
    //        for (i = 0; i < dim; i++) {
    //            for (j = jj; j < min(jj+BLOCK,dim); j++) {
    //                sum = 0.0;
    //
    //                for (k = kk; k < min(kk+BLOCK, dim); k++) {
    //                    sum += a[i*dim+k] * b[k*dim+j];
    //                }
    //                c[i*dim+j] += sum;
    //            }
    //        }
    //    }

    return NULL; 
}

void parallelMatrixMultiply(int * aa, int * bb, int * cc, int dimdim) {

    //assert(THREADS == pow(dimdim / BLOCK, 2));
    int i = 0;
    pthread_t threads[THREADS];
    int inputs[THREADS];

    a = aa;
    b = bb;
    c = cc;
    dim = dimdim;

    for(i= 1; i < THREADS; i++){
        inputs[i] = i;
        pthread_create(&(threads[i]), NULL, parallelMatrixMultiply_thread, &(inputs[i]));
    }

    inputs[0] = 0;
    parallelMatrixMultiply_thread(&inputs[0]);

    for (i = 1; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

}

/*
   double wrapper_func(){
   int i = 0;
   pthread_t threads[t];
   int inputs[t];

   for(i= 1; i < t; i++){
   inputs[i] = i;
   pthread_create(&(threads[i]), NULL, parallelMatrixMultiply, &(inputs[i]));
   }

   inputs[0] = 0;
   parallelMatrixMultiply_thread(&inputs[0]);

   for (i = 1; i < t; i++) {
   pthread_join(threads[i], NULL);
   }

   struct timespec min = start[0];
   struct timespec max = end[0];


   for(i = 0; i < t; i++) {
   if(start[i].tv_sec < min.tv_sec || 
   (start[i].tv_sec == min.tv_sec && start[i].tv_nsec < min.tv_nsec)) {
   min = start[i];
   }
   } 

   for(i = 0; i < t; i++) {
   if(end[i].tv_sec > max.tv_sec || 
   (end[i].tv_sec == max.tv_sec && end[i].tv_nsec > max.tv_nsec)) {
   max = end[i];
   }
   } 

   double fixed_time = ((double)((max.tv_sec-min.tv_sec)+(max.tv_nsec-min.tv_nsec)/1e9));
   return fixed_time;

   } 
   */

int main() {

    int base_dim = max(THREADS * BLOCK, 64);

    int j = 0, k = 0;
    for (j = 1; j < 16; j++) {

        int *a, *b, *c;

        int i = 0;
        int dim = j*base_dim;

        struct timespec start_t, end_t;

        a = (int *)calloc(dim * dim, sizeof(int));
        b = (int *)calloc(dim * dim, sizeof(int));
        c = (int *)calloc(dim * dim, sizeof(int));

        double fixed_time = 0;
        for (i = 0; i < dim * dim; i++)  {
            a[i] = i;
            b[i] = i;
            c[i] = i;
        }

        for (k = 0; k < 100; k++) {
            clock_gettime(CLOCK_REALTIME, &start_t);
            parallelMatrixMultiply(a, b, c, dim);
            clock_gettime(CLOCK_REALTIME, &end_t);
            fixed_time += ((double)((end_t.tv_sec-start_t.tv_sec)+(end_t.tv_nsec-start_t.tv_nsec)/1e9));
        }

        double size = (dim * dim * sizeof(int)) / 1e6;
        printf("%d, %g\n", dim, (double) size / (fixed_time/100));

        free(a);
        free(b);
        free(c);
    }
    return 0;
}
