#include <assert.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 4096
char buffer[BUFFER_SIZE];

void initialize_buffer(void)
{
    int i;
    char *pattern = "0123456789abcdefghijlmnopqrstuvw";

    for (i = 0; i < BUFFER_SIZE; i += 32) {
        memcpy(&buffer[i], pattern, 32);
    }
}

void do_test(size_t dest_start, size_t dest_len, size_t src_start, size_t src_len)
{
    char dest_end;
    char src_end;
    char copied_end;
    char *dest;
    char *src;
    int tmp;

    assert(dest_start + dest_len + src_len < src_start);
    assert(src_start + src_len < BUFFER_SIZE);

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

    strcat(dest, src);

    for (tmp = dest_start + dest_len; tmp < dest_start + dest_len + src_len; ++tmp) 
    {
        assert(buffer[tmp] == buffer[src_start + tmp - dest_start - dest_len]);
    }

    dest[dest_len] = dest_end;
    src[src_len] = src_end;
    dest[dest_len + src_len] = copied_end;
}

int main(void)
{
    /* Begin by initializing the buffer.  This also has the desirable
     * side-effect of warming up the cache.  Always do this before you
     * begin any timing runs.  */
    initialize_buffer();

    /* Time calls to strcat of various lengths. */
    do_test(0, 1, 2048, 1);
    do_test(0, 1, 2048, 4);
    do_test(0, 1, 2048, 19);
    do_test(0, 1, 2048, 103);
    do_test(0, 1, 2048, 1900);
    do_test(1, 4, 2048, 1);
    do_test(1, 4, 2048, 4);
    do_test(1, 4, 2048, 19);
    do_test(1, 4, 2048, 103);
    do_test(1, 4, 2048, 1900);
    do_test(4, 32, 2048, 1);
    do_test(4, 32, 2048, 4);
    do_test(4, 32, 2048, 19);
    do_test(4, 32, 2048, 103);
    do_test(4, 32, 2048, 1900);

    return 0;
}
