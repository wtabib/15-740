#include <stdio.h>
#include <iostream>

#define ELEM 100

int arr[100];

void store(int n, int i){
    arr[i] = n;
}


int load(int n){
    return arr[n];
}




int main() {

    int i;
    int a;
    int arr[100];

    for( i = 0; i < 100; i++){
        store(i, i);
    }

    for( i = 0; i < 100; i++){
        a = load(i);
    }

    return 1;
}
