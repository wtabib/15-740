#include "library.h"

void store(int n, int i, int *arr){
    *(arr+i) = n;
}


int load(int n, int *arr){
    return *(arr+n);
}
