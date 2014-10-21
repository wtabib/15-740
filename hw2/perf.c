/**
 * @file perf.c
 * @brief Measure performance: interval timer and hardware counter wrappers
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include "perf.h"

//avoid header conflict
#define CLOCK_MONOTONIC_RAW 4

/* static variable that holds the initial value of the interval timer */
struct itimerval first_u; /* user time */
struct itimerval first_r; /* real time */
struct timespec first_h; /* hardware time */
int perf_fd; /* perf events */

/*
 * elapsed user time routines
 */

/* init the timer */
void init_etime(void)
{
    first_u.it_interval.tv_sec = 0;
    first_u.it_interval.tv_usec = 0;
    first_u.it_value.tv_sec = MAX_ETIME;
    first_u.it_value.tv_usec = 0;
    setitimer(ITIMER_VIRTUAL, &first_u, NULL);
}

/* return elapsed seconds since call to init_etime */
long double get_etime(void) {
    struct itimerval curr;

    getitimer(ITIMER_VIRTUAL, &curr);
    return (long double) ((first_u.it_value.tv_sec - curr.it_value.tv_sec) +
         (first_u.it_value.tv_usec - curr.it_value.tv_usec)*1e-6);
}

/*
 * elapsed real time routines
 */

/* init the timer */
void init_etime_real(void)
{
    first_r.it_interval.tv_sec = 0;
    first_r.it_interval.tv_usec = 0;
    first_r.it_value.tv_sec = MAX_ETIME;
    first_r.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &first_r, NULL);
}

/* return elapsed seconds since call to init_etime_real */
long double get_etime_real(void) {
    struct itimerval curr;

    getitimer(ITIMER_REAL, &curr);
    return (long double) ((first_r.it_value.tv_sec - curr.it_value.tv_sec) +
		     (first_r.it_value.tv_usec - curr.it_value.tv_usec)*1e-6);
}

/*
 * Hardware clock routines
 */
void init_etime_hw(void)
{
    clock_gettime(CLOCK_MONOTONIC_RAW, &first_h);
}

/* return elapsed seconds since call to init_etime_real */
long double get_etime_hw(void)
{
    struct timespec curr;

    clock_gettime(CLOCK_MONOTONIC_RAW, &curr);

    return (long double) ((curr.tv_sec - first_h.tv_sec) +
        (curr.tv_nsec - first_h.tv_nsec)*1e-9);
}

long perf_event_open( struct perf_event_attr *hw_event, pid_t pid,
                      int cpu, int group_fd, unsigned long flags )
{
    int ret;

    ret = syscall( __NR_perf_event_open, hw_event, pid, cpu,
                   group_fd, flags );
    return ret;
}

/*
 * start counting cache misses
 */
#define CACHE_L1D_READ_MISS         (PERF_COUNT_HW_CACHE_L1D | PERF_COUNT_HW_CACHE_OP_READ << 8 | PERF_COUNT_HW_CACHE_RESULT_MISS << 16)

void start_cachemiss_count(void)
{
    struct perf_event_attr pe;
    memset(&pe, 0, sizeof(struct perf_event_attr));
    pe.type = PERF_TYPE_HW_CACHE;
    pe.size = sizeof(struct perf_event_attr);
    pe.config = CACHE_L1D_READ_MISS;
    pe.disabled = 1;
    pe.exclude_kernel = 1;
    pe.exclude_hv = 1;
    perf_fd = perf_event_open(&pe, 0, -1, -1, 0);
    ioctl(perf_fd, PERF_EVENT_IOC_RESET, 0);
    ioctl(perf_fd, PERF_EVENT_IOC_ENABLE, 0);
}

/*
 * return the total number of cache misses after
 * start_cachemiss_count
 */
long long get_cachemiss_count(void)
{
    long long count;
    ioctl(perf_fd, PERF_EVENT_IOC_DISABLE, 0);
    read(perf_fd, &count, sizeof(long long));
    close(perf_fd);

    return count;
}
