#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 500
#define K 500
#define M 500
/* note: if experiencing out-of-memory issues, scale down the variables*/

/* multiply a[NxK] x b[KxM] to C[NxM]   */

int a[N][K], b[K][M], c[N][M];

void initialize() {
    register int i, j, random_base;

    srand(time(NULL));
    random_base = rand();

    for (i = 0; i < N; i++) {
        for (j = 0; j < K; j++) {
            a[i][j] = (i+j+random_base) & 0xF;
        }
    }

    srand(time(NULL));
    random_base = rand();

    for (i = 0; i < K; i++) {
        for (j = 0; j < M; j++) {
            b[i][j] = (i+j+random_base) & 0xF;
        }
    }

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            c[i][j] = 0;
        }
    }

}

void MatrixMatrixMultiplication() {
    register int i, j, k;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            for (k = 0; k < K; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

int main (int argc, char ** argv) {
    printf("Initializing\n");
    initialize();
    printf("Multiplying\n");
    MatrixMatrixMultiplication();
    printf("Done!\n");
    return 0;
}
