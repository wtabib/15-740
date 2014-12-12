#ifndef DIRECTORYCACHE_H
#define DIRECTORYCACHE_H

#define NUM_ELEM        100
#define NUM_THREADS     50
#define     N_CORES     5

//define the number of clock cycles for part
#define WAIT_TIME       1
#define BUS_TRANSFER    2
#define LOOKUP          5
#define FLUSH           7

int load(int i, int *readFromArr);
void store(int* writeToArr, int data, int i);

typedef struct {
    int w[NUM_ELEM];
    int r[NUM_ELEM];
} Arr;

int updateDirectoryRead(int pid, int*addr);
int updateDirectoryWrite(int pid, int* addr);

#endif
