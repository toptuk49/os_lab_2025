#ifndef CLIENT_ARGUMENTS_H
#define CLIENT_ARGUMENTS_H

#include <stdbool.h>

typedef struct
{
  char *ip;
  int port;
  int buffer_size;
} ProgramArguments;

bool parse_arguments(int *argc, char ***argv, ProgramArguments *arguments);
void free_program_arguments(ProgramArguments *arguments);

#endif  // CLIENT_ARGUMENTS_H
