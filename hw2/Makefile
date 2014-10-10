CC = gcc
AS = as
CFLAGS = -O
LFLAGS =  -lrt -lm
		 
all: mountain

func_time.o: func_time.c func_time.h
	$(CC) $(CFLAGS) -c func_time.c

perf.o: perf.c perf.h
	$(CC) $(CFLAGS) -c perf.c

mountain: mountain.c func_time.o perf.o
	$(CC) $(CFLAGS) mountain.c perf.o func_time.o -o mountain.o $(LFLAGS) 
