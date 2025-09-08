CC=gcc
CFLAGS=-I.

all: parallel_min_max

run_parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o run_parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

clean :
	rm utils.o find_min_max.o parallel_min_max
