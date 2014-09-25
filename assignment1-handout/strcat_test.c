#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "func_time.h"
#include "perf.h"
#include "strcat_naive.h"

#define BUFFER_SIZE 4096
char buffer[BUFFER_SIZE];

typedef long double (*timer_funct)(test_funct, long double);
typedef void (*strcat_funct)(void);
size_t dest_start;
size_t dest_len; 
size_t src_start;
size_t src_len;
timer_funct timer; 
strcat_funct concat;

void initialize_buffer(void)
{
    int i;
    char *pattern = "0123456789abcdefghijlmnopqrstuvw";

    for (i = 0; i < BUFFER_SIZE; i += 32) {
        memcpy(&buffer[i], pattern, 32);
    }
}

char *gSrc; char *gDst;
void strcatWrapper() {
    strcat(gDst, gSrc);
}

void naiveStrcatWrapper() {
    my_strcat(gDst, gSrc);
}


int ax = 1;
int bx = 1;
void add_dummy(void)
{
    int a = ax;
    int b = bx;
    ax = a + b;
}



void do_test(size_t dest_start, size_t dest_len, size_t src_start, size_t src_len, strcat_funct concat)
{
    char dest_end;
    char src_end;
    char copied_end;
    char *dest;
    char *src;
    int tmp;

    //assert(dest_start + dest_len + src_len < src_start);
    //assert(src_start + src_len < BUFFER_SIZE);

    dest = buffer + dest_start;
    dest_end = dest[dest_len];
    dest[dest_len] = '\0';

    src = buffer + src_start;
    src_end = src[src_len];
    src[src_len] = '\0';

    copied_end = dest[dest_len + src_len];

    /* This is the call to strlen which we want to time.  Insert your
     * timing code here. (You may have to add some wrapper code and
     * get rid of the sanity checks.) */

    gDst = dest; gSrc = src;
    concat();
    //long double secs = timer(concat, 100);// - timer(add_dummy, 0.02);
    //printf("number of seconds %Lf\n", secs);

    /*for (tmp = dest_start + dest_len; tmp < dest_start + dest_len + src_len; ++tmp) 
      {
      assert(buffer[tmp] == buffer[src_start + tmp - dest_start - dest_len]);
      }*/

    dest[dest_len] = dest_end;
    src[src_len] = src_end;
    dest[dest_len + src_len] = copied_end;
}

long long do_cache_test(size_t dest_start, size_t dest_len, size_t src_start, size_t src_len, strcat_funct concat)
{
    char dest_end;
    char src_end;
    char copied_end;
    char *dest;
    char *src;
    int tmp;

    //assert(dest_start + dest_len + src_len < src_start);
    //assert(src_start + src_len < BUFFER_SIZE);

    dest = buffer + dest_start;
    dest_end = dest[dest_len];
    dest[dest_len] = '\0';

    src = buffer + src_start;
    src_end = src[src_len];
    src[src_len] = '\0';

    copied_end = dest[dest_len + src_len];

    /* This is the call to strlen which we want to time.  Insert your
     * timing code here. (You may have to add some wrapper code and
     * get rid of the sanity checks.) */

    gDst = dest; gSrc = src;
    start_cachemiss_count();
    concat();

    long long numMisses = get_cachemiss_count();

    //long double secs = timer(concat, 100);// - timer(add_dummy, 0.02);
    //printf("number of seconds %Lf\n", secs);

    /*for (tmp = dest_start + dest_len; tmp < dest_start + dest_len + src_len; ++tmp) 
      {
      assert(buffer[tmp] == buffer[src_start + tmp - dest_start - dest_len]);
      }*/

    dest[dest_len] = dest_end;
    src[src_len] = src_end;
    dest[dest_len + src_len] = copied_end;

    return numMisses;
}



void doTestWrapper() {
    do_test(dest_start, dest_len, src_start, src_len, concat);
}

int main(void)
{
    /* Begin by initializing the buffer.  This also has the desirable
     * side-effect of warming up the cache.  Always do this before you
     * begin any timing runs.  */
    initialize_buffer();

    /* Time calls to strcat of various lengths. */
    int i = 0, j = 0, k = 0;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            if (i == 0) 
                timer= func_time;
            else
                timer= func_time_hw;

            if (j == 0)
                concat= strcatWrapper;
            else
                concat= naiveStrcatWrapper;


            //do_test(0, 1, 2048, 1, timer_main, concat_main);
            int iterations = 1;
            dest_start = 0; dest_len = 1; src_start = 2048; src_len = 1;
            long long miss[100];

            for (k = 0; k < 100; k++) {
                miss[k] = 0;
            }

            for (k = 0; k < iterations; k++) {

                miss[0] += do_cache_test(0, 1, 2048, 4, concat);
                miss[1] += do_cache_test(0, 1, 2048, 4,  concat);
                miss[2] += do_cache_test(0, 1, 2048, 19,  concat);
                miss[3] += do_cache_test(0, 1, 2048, 103,  concat);
                miss[4] += do_cache_test(0, 1, 2048, 1900,  concat);
                miss[5] += do_cache_test(1, 4, 2048, 1,  concat);
                miss[6] += do_cache_test(1, 4, 2048, 4,  concat);
                miss[7] += do_cache_test(1, 4, 2048, 19,  concat);
                miss[8] += do_cache_test(1, 4, 2048, 103,  concat);
                miss[9] += do_cache_test(1, 4, 2048, 1900,  concat);
                miss[10] += do_cache_test(4, 32, 2048, 1,  concat);
                miss[11] += do_cache_test(4, 32, 2048, 4,  concat);
                miss[12] += do_cache_test(4, 32, 2048, 19,  concat);
                miss[13] += do_cache_test(4, 32, 2048, 103,  concat);
                miss[14] += do_cache_test(4, 32, 2048, 1900,  concat);
            }

            for (k = 0; k < 15; k++) {
                printf("miss[%d] = %g\n", k, miss[k]/((double)iterations));
            }



            //do_test(0, 1, 2048, 1, timer_main, concat_main);
            dest_start = 0; dest_len = 1; src_start = 2048; src_len = 1;
            long double secs = timer(doTestWrapper, 0.005) - timer(add_dummy, 0.005);
            printf("number of seconds %Le\n", secs);

            //do_test(0, 1, 2048, 4, timer_main, concat_main);
            dest_start = 0; dest_len = 1; src_start = 2048; src_len = 4;
            secs = timer(doTestWrapper, 0.005) - timer(add_dummy, 0.005);
            printf("number of seconds %Le\n", secs);

            //do_test(0, 1, 2048, 19, timer_main, concat_main);
            dest_start = 0; dest_len = 1; src_start = 2048; src_len = 19;
            secs = timer(doTestWrapper, 0.005) - timer(add_dummy, 0.005);
            printf("number of seconds %Le\n", secs);

            //do_test(0, 1, 2048, 103, timer_main, concat_main);
            dest_start = 0; dest_len = 1; src_start = 2048; src_len = 103;
            secs = timer(doTestWrapper, 0.005) - timer(add_dummy, 0.005);
            printf("number of seconds %Le\n", secs);

            //do_test(0, 1, 2048, 1900, timer_main, concat_main);
            dest_start = 0; dest_len = 1; src_start = 2048; src_len = 1900;
            secs = timer(doTestWrapper, 0.005) - timer(add_dummy, 0.005);
            printf("number of seconds %Le\n", secs);

            //do_test(1, 4, 2048, 1, timer_main, concat_main);
            dest_start = 1; dest_len = 4; src_start = 2048; src_len = 1;
            secs = timer(doTestWrapper, 0.005) - timer(add_dummy, 0.005);
            printf("number of seconds %Le\n", secs);

            //do_test(1, 4, 2048, 4, timer_main, concat_main);
            dest_start = 1; dest_len = 4; src_start = 2048; src_len = 4;
            secs = timer(doTestWrapper, 0.005) - timer(add_dummy, 0.005);
            printf("number of seconds %Le\n", secs);

            //do_test(1, 4, 2048, 19, timer_main, concat_main);
            dest_start = 1; dest_len = 4; src_start = 2048; src_len = 19;
            secs = timer(doTestWrapper, 0.005) - timer(add_dummy, 0.005);
            printf("number of seconds %Le\n", secs);

            //do_test(1, 4, 2048, 103, timer_main, concat_main);
            dest_start = 1; dest_len = 4; src_start = 2048; src_len = 103;
            secs = timer(doTestWrapper, 0.005) - timer(add_dummy, 0.005);
            printf("number of seconds %Le\n", secs);

            //do_test(1, 4, 2048, 1900, timer_main, concat_main);
            dest_start = 1; dest_len = 4; src_start = 2048; src_len = 1900;
            secs = timer(doTestWrapper, 0.005) - timer(add_dummy, 0.005);
            printf("number of seconds %Le\n", secs);

            //do_test(4, 32, 2048, 1, timer_main, concat_main);
            dest_start = 4; dest_len = 32; src_start = 2048; src_len = 1;
            secs = timer(doTestWrapper, 0.005) - timer(add_dummy, 0.005);
            printf("number of seconds %Le\n", secs);

            //do_test(4, 32, 2048, 4, timer_main, concat_main);
            dest_start = 4; dest_len = 32; src_start = 2048; src_len = 4;
            secs = timer(doTestWrapper, 0.005) - timer(add_dummy, 0.005);
            printf("number of seconds %Le\n", secs);

            //do_test(4, 32, 2048, 19, timer_main, concat_main);
            dest_start = 4; dest_len = 32; src_start = 2048; src_len = 19;
            secs = timer(doTestWrapper, 0.005) - timer(add_dummy, 0.005);
            printf("number of seconds %Le\n", secs);

            //do_test(4, 32, 2048, 103, timer_main, concat_main);
            dest_start = 4; dest_len = 32; src_start = 2048; src_len = 103;
            secs = timer(doTestWrapper, 0.005) - timer(add_dummy, 0.005);
            printf("number of seconds %Le\n", secs);

            //do_test(4, 32, 2048, 1900, timer_main, concat_main);
            dest_start = 4; dest_len = 32; src_start = 2048; src_len = 1900;
            secs = timer(doTestWrapper, 0.005) - timer(add_dummy, 0.005);
            printf("number of seconds %Le\n", secs);


            /////////////////////////////////////////////////////////////////////
            printf("****************switching concat*******************\n");
        }
        printf("****************switching timer*******************\n");
    }

    return 0;
}

