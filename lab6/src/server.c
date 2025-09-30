#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <getopt.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "../libs/safe_modulo_multiplication.h"
#include "../libs/server_arguments.h"
#include "pthread.h"

typedef struct {
  uint64_t start;
  uint64_t end;
} Range;

void *ThreadFunction(void *arg);
uint64_t FactorialRange(Range *range);

ProgramArguments args = {.tnum = -1, .port = -1, .mod = -1};
uint64_t result = 1;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv) {
  if (!parseArguments(&argc, &argv, &args))
    return 1;

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    fprintf(stderr, "Can not create server socket!");
    return 1;
  }

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons((uint16_t)args.port);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  int opt_val = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val));

  int err = bind(server_fd, (struct sockaddr *)&server, sizeof(server));
  if (err < 0) {
    printf("Can not bind to socket!");
    return 1;
  }

  err = listen(server_fd, 128);
  if (err < 0) {
    printf("Could not listen on socket\n");
    return 1;
  }

  printf("Server listening at %d\n", args.port);

  while (true) {
    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);
    int client_fd = accept(server_fd, (struct sockaddr *)&client, &client_len);

    if (client_fd < 0) {
      printf("Could not establish new connection\n");
      continue;
    }

    while (true) {
      unsigned int buffer_size = sizeof(uint64_t) * 3;
      char from_client[buffer_size];
      int read = recv(client_fd, from_client, buffer_size, 0);

      if (!read)
        break;
      if (read < 0) {
        printf("Client read failed\n");
        break;
      }
      if (read < buffer_size) {
        printf("Client send wrong data format\n");
        break;
      }

      uint64_t begin = 0;
      uint64_t end = 0;
      uint64_t mod = 0;
      memcpy(&begin, from_client, sizeof(uint64_t));
      memcpy(&end, from_client + sizeof(uint64_t), sizeof(uint64_t));
      memcpy(&mod, from_client + 2 * sizeof(uint64_t), sizeof(uint64_t));

      pthread_t threads[args.tnum];
      Range ranges[args.tnum];

      unsigned long long chunk = end / args.tnum;
      unsigned long long start = 1;

      for (int i = 0; i < args.tnum; i++) {
        ranges[i].start = start;
        ranges[i].end = (i == args.tnum - 1) ? end : (start + chunk - 1);
        start = ranges[i].end + 1;

        pthread_create(&threads[i], NULL, ThreadFunction, &ranges[i]);
      }

      for (int i = 0; i < args.tnum; i++) {
        pthread_join(threads[i], NULL);
      }

      char buffer[sizeof(result)];
      memcpy(buffer, &result, sizeof(result));
      err = send(client_fd, buffer, sizeof(result), 0);
      if (err < 0) {
        printf("Can't send data to client\n");
        break;
      }
    }

    shutdown(client_fd, SHUT_RDWR);
    close(client_fd);
  }

  return 0;
}

uint64_t FactorialRange(Range *range) {
  uint64_t accumulator = 1;
  for (uint64_t i = range->start; i <= range->end; i++) {
    accumulator = SafeModuloMultiplication(accumulator, i, args.mod);
  }

  return accumulator;
}

void *ThreadFunction(void *arg) {
  Range *range = (Range *)arg;
  unsigned long long partial = FactorialRange(range);

  pthread_mutex_lock(&lock);
  result = SafeModuloMultiplication(result, partial, args.mod);
  pthread_mutex_unlock(&lock);

  return NULL;
}
