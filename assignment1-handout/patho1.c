#include <stdio.h>

long long int main() {
    
    unsigned int numElem = 10000;
    long long int arr0[numElem];
    long long int arr1[numElem];

    int i;
    for (i = 0; i < numElem; i++) {
        arr0[i] = i%37;
        arr1[i] = i%42;
    } 

    long long int sum1 = 0, sum2 = 0, sum3 = 0;
    int j = 0, k = 0;
    for (i = 0; i < numElem; i++) {
        for ( j = 0; j < numElem -i; j++) {
            sum1 += arr0[j];

        }

        for (k = 0; k < numElem - i; k++) {
            sum2 += arr1[k];
        }

        sum3 = sum1 - sum2;
    }

    return sum3;
}
