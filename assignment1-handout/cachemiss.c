#include <stdio.h>
#include "perf.h"
#include <stdlib.h>

void flushCache() {
   int i = 0;
   int numElem = 15000*10000;
   int* arr3 = (int *) malloc(numElem*sizeof(int));
   int* arr4 = (int *) malloc(numElem*sizeof(int));

   for (i = 0; i < numElem; i++) {
       arr3[i] = i;
       arr4[i] = i;
   }
   for (i = 0; i < numElem; i++) {
       arr3[i] = arr4[i];
   }
}

long long thing1() {

   int i = 0;
   int numElem = 15000;
   flushCache();
   int* arr1 = (int *) malloc(numElem*sizeof(int));
   int* arr2 = (int *) malloc(numElem*sizeof(int));
   long long numMisses = 0;

    start_cachemiss_count();
    for (i = 0; i < numElem; i++) {
        arr1[i] = i;
        arr2[i] = numElem-i;

        int elem = arr1[i];
    }

    numMisses = get_cachemiss_count();

    //free(arr1);
    //free(arr2);

    return numMisses;
}

long long thing2() {

   int i = 0;
   int numElem = 15000;
   flushCache();
   int* arr1 = (int *) malloc(numElem*sizeof(int));
   int* arr2 = (int *) malloc(numElem*sizeof(int));
   long long numMisses = 0;

   start_cachemiss_count();

   for (i = 0; i < numElem; i++) {
        arr1[i%15] = i;
   }

   for (i = 0; i < numElem; i++) {
        arr2[i%15] = numElem-i;
   }

   for (i = 0; i < numElem; i++) {
        int elem = arr1[i%15];
   }

   numMisses = get_cachemiss_count();
   //free(arr1);
   //free(arr2);

   return numMisses;
}


int main() {
  
    int i = 0;
    int numCalls = 10;
    long long total1 = 0, total2 = 0;
    /*
   for (i = 0; i < numCalls; i++) {
       long long numMisses1 = thing1();
       long long numMisses2 = thing2();

       total1 += numMisses1;
       total2 += numMisses2;
   }*/

   numCalls = 100;
   total1 = 0; total2 = 0;

   for (i = 0; i < numCalls; i++) {
       long long numMisses1 = thing1();
       long long numMisses2 = thing2();

       total1 += numMisses1;
       total2 += numMisses2;
   }

   printf("total for thing1 = %f\n", ((float)total1)/((float)numCalls));
   printf("total for thing2 = %f\n", ((float)total2)/((float)numCalls));

   return 0;
  
}


