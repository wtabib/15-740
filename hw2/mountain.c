#include "func_time.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define CMAX 1024L*1024L*512L
#define SMAX 256

char* memory;

unsigned long long int n, s;

void better_measure(){
    double result = 0.0;
    volatile double sink;
    unsigned long long int i;
    unsigned long long int j;
    unsigned long long numrows = 32;
    unsigned long long rowsize = (n * s) / numrows;

    for (i=0; i<n/numrows; i++){
        for(j = 0; j < numrows; j++) {
            result = result + (double)memory[((i*s) + (j * rowsize))%(1024L*1024L*512L)];
        }
    }
    sink = result;
}


void simple_measure(){
    double result = 0.0;
    volatile double sink;
    unsigned long long int i;
    for (i=0; i<n; i++){
        result = result + (double)memory[(i*s)%(1024L*1024L*512L)];
    }
    sink = result;
}


void main(int argc, char** argv){
    
    unsigned long long int i;

    unsigned long long int tot;
    
    memory = (char*)calloc(CMAX, 1);
    tot = SMAX;
    tot = tot * CMAX;

    /*for(i=0; i<1024L*1024L*512L; i++){
        memory[i] = 'a'; // (char)(i%256);
    }*/

    long double secs;
    unsigned long long int j;
    for(j = 1024L; j<CMAX; j=j*4){

         for (i=0; i<=SMAX; i=i+4){
             s = i;
             n = j;
         
             secs = func_time(better_measure, 0.02); // - func_time(add_dummy, 0.02);
             printf("%Ld\t %f\t %Le\n", s, log2(j), n/(secs*1e6));
         }
    }


}
