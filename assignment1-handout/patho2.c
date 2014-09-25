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
    unsigned long long int counter1 = 97;
    unsigned long long int counter2 = 13;
    for (i = 0; i < numElem; i++) {
        for ( j = 0; j < numElem -i; j++) {
            counter1 = counter1*counter1;
            sum1 += arr0[counter1%numElem];

        }

        for (k = 0; k < numElem - i; k++) {
            counter2 = counter2*counter2;
            sum2 += arr1[counter2%numElem];
        }

        sum3 = sum1 - sum2;
    }

    return sum3;
}
