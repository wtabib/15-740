#include "func_time.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define CMAX 1024L*1024L*1024L
#define SMAX 25L

char* memory;

int n, s;

void simple_measure(){
    double result = 0.0;
    volatile double sink;
    int i;
    for (i=0; i<n; i++){
        result = result + (double)memory[i*s];
    }
    sink = result;
}


void main(int argc, char** argv){
    
    unsigned long long int i;

    unsigned long long int tot;
    
    memory = (char*)malloc(CMAX*SMAX);
    tot = SMAX;
    tot = tot * CMAX;

    /*for(i=0; i<tot; i++){
        memory[i] = (char)(i%256);
    }*/

    long double secs;
    unsigned long long int j;
    for(j = 1024*1024*64; j<CMAX; j=j*2){

         for (i=1; i<SMAX; i++){
             s = i;
             n = j;
         
             secs = func_time(simple_measure, 0.02); // - func_time(add_dummy, 0.02);
             printf("%d\t %f\t %Le\n", s, log2(j), n/(secs*1e6));
         }
    }


}
