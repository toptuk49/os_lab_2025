CC=gcc
CFLAGS=-I. -std=c99 -pthread

all: mutex without_mutex

mutex:
	$(CC) mutex.c -o mutex $(CFLAGS)

without_mutex:
	$(CC) without_mutex.c -o without_mutex $(CFLAGS)

clean:
	rm mutex without_mutex
