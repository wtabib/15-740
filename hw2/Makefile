CC = gcc
AS = as
CFLAGS = -O
LFLAGS =  -lrt -lm
		 
all: mountain core linesize

func_time.o: func_time.c func_time.h
	$(CC) $(CFLAGS) -c func_time.c

perf.o: perf.c perf.h
	$(CC) $(CFLAGS) -c perf.c

mountain: mountain.c func_time.o perf.o
	$(CC) $(CFLAGS) mountain.c perf.o func_time.o -o mountain.o $(LFLAGS) 

core: core.c perf.o func_time.o
	$(CC) -g $(CFLAGS) core.c perf.o func_time.o -o core -lpthread 

clock_test: pthread_clock_test.c
	$(CC) -g $(CFLAGS) pthread_clock_test.c -o clock_test -lpthread -lrt

linesize: linesize.c linesize.o func_time.o
	$(CC) -g $(CFLAGS) linesize.c perf.o func_time.o -o linesize -lpthread 
