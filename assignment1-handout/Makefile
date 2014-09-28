CC = gcc
AS = as
CFLAGS = -O
LFLAGS =  -lrt -lm

CACHE_TEST_SRCS = cache_test/MMM.c cache_test/NN.c cache_test/Norm1.c cache_test/Norm2.c
CACHE_TEST_TARGS = $(patsubst cache_test/%.c,cache_test/%.out,$(CACHE_TEST_SRCS))
all: example freq cache_tests strcat_opt strcat_test

strcat_opt: strcat_opt.c
	$(CC) $(CFLAGS) -O3 -c strcat_opt.c

cache_tests: $(CACHE_TEST_TARGS)

example: example.c perf.h perf.o
	$(CC) $(CFLAGS) -o example example.c perf.o $(LFLAGS)

strcat_test: strcat_test.c perf.h perf.o func_time.h func_time.o strcat_naive.h strcat_naive.o strcat_opt.h strcat_opt.o
	$(CC) $(CFLAGS) -o strcat_test  strcat_test.c perf.o func_time.o strcat_naive.o strcat_opt.o $(LFLAGS)

freq: freq.c perf.h func_time.h perf.o func_time.o
	$(CC) $(CFLAGS) -o freq freq.c perf.o func_time.o $(LFLAGS)

perf.o: perf.c perf.h
	$(CC) $(CFLAGS) -c perf.c

func_time.o: func_time.c func_time.h
	$(CC) $(CFLAGS) -c func_time.c

cachemiss: cachemiss.c perf.h perf.o
	$(CC) $(CFLAGS) -o cachemiss cachemiss.c perf.o $(LFLAGS)

submit:
	tar cvzf submission.tar.gz writeup.txt func_time.c strcat-x64-annotated.dis strcat\_opt.c patho1.c patho1.txt patho2.txt patho2.c pintool

clean:
	rm -f *.o example freq *~

