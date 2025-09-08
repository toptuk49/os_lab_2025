CC=gcc
CFLAGS=-I. -std=c99 # Standard to work with 64-bit numbers

all: parallel_min_max

parallel_min_max: utils.o find_min_max.o parallel_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.o $(CFLAGS)

utils.o: utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o: utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

parallel_min_max.o: parallel_min_max.c
	$(CC) -o parallel_min_max.o -c parallel_min_max.c $(CFLAGS)

clean:
	rm *.o parallel_min_max
