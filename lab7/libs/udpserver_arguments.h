#ifndef UDPSERVER_ARGUMENTS_H
#define UDPSERVER_ARGUMENTS_H

#include <stdbool.h>

typedef struct
{
  int port;
  int buffer_size;
} ProgramArguments;

bool parse_arguments(int *argc, char ***argv, ProgramArguments *arguments);

#endif  // UDPSERVER_ARGUMENTS_H
