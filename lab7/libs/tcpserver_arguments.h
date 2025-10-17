#ifndef TCPSERVER_ARGUMENTS_H
#define TCPSERVER_ARGUMENTS_H

#include <stdbool.h>

typedef struct
{
  int port;
  int buffer_size;
  int backlog;
} ProgramArguments;

bool parse_arguments(int *argc, char ***argv, ProgramArguments *arguments);

#endif  // TCPSERVER_ARGUMENTS_H
