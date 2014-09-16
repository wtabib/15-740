#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000
#define M 1000
/* note: if experiencing out-of-memory issues, scale down the variables*/

int a[N][M], b[N][M];
/* nearest neighbor average
   b[i][j] = (a[i][j] + a[i-1][j] + a[i+i][j] + a[i][j-1] + a[i][j+1])/5
   but only for the internal ones (we don't care about the edge)
*/

void initialize() {
    register int i, j, random_base;

    srand(time(NULL));
    random_base = rand();

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            a[i][j] = (i+j+random_base) & 0xF;
        }
    }

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            b[i][j] = 0; //not necessary, but for cache warming
        }
    }

}

void NearestNeighborAverage() {
    register int i, j;

    for (i = 1; i < N-1; i++) {
        for (j = 1; j < M-1; j++) {
            b[i][j] = ( a[i][j] +
                        a[i-1][j] + a[i+1][j] +
                        a[i][j-1] + a[i][j+1]
                       )/5;
        }
    }
}

int main (int argc, char ** argv) {
    printf("Initializing\n");
    initialize();
    printf("Average\n");
    NearestNeighborAverage();
    printf("Done!\n");
    return 0;
}
