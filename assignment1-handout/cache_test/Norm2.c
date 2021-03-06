#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* NOTE: Must add -lm when compiling to link math library */

#define N 10000
#define M 10000
/* note: if experiencing out-of-memory issues, scale down the variables*/


/*  calculates frobenius norm
    sqrt(Sum(Sum(a_ij^2,i),j))
    sum over j
        sum over i
*/

int a[N][M];

void initialize() {
    register int i, j, random_base;

    srand(time(NULL));
    random_base = rand();

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            a[i][j] = (i+j+random_base) & 0xF;
        }
    }
}

double FrobeniusNorm2() {
    register int i, j;
    register double sum = 0.0, temp;

    for (j = 0; j < M; j++) {
        for (i = 0; i < N; i++) {
            temp = (double) a[i][j];
            sum += temp*temp ;
        }
    }
    return (sqrt(sum));
}

int main (int argc, char ** argv) {
    double norm;
    printf("Initializing\n");
    initialize();
    printf("Calculating\n");
    norm = FrobeniusNorm2();
    printf("Done! Norm is %lf\n", norm);
    return 0;
}
