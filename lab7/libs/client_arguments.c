#include "client_arguments.h"

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE 10
#define DEFAULT_BUFSIZE 100
#define MAX_PORT 65536

bool parse_arguments(int *argc, char ***argv, ProgramArguments *arguments)
{
  arguments->ip = NULL;
  arguments->port = -1;
  arguments->buffer_size = DEFAULT_BUFSIZE;

  while (true)
  {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"ip", required_argument, 0, 0},
                                      {"port", required_argument, 0, 0},
                                      {"bufsize", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int option_char = getopt_long(*argc, *argv, "", options, &option_index);

    if (option_char == -1)
    {
      break;
    }

    switch (option_char)
    {
      case 0:
      {
        switch (option_index)
        {
          case 0:
            arguments->ip = malloc(strlen(optarg) + 1);
            if (arguments->ip == NULL)
            {
              printf("Memory allocation failed for IP address!\n");
              return false;
            }
            strcpy(arguments->ip, optarg);
            break;
          case 1:
            arguments->port = (int)strtol(optarg, NULL, BASE);
            if (arguments->port <= 0 || arguments->port > MAX_PORT)
            {
              printf("Port must be in range 1-65535\n");
              return false;
            }

            break;
          case 2:
            arguments->buffer_size = (int)strtol(optarg, NULL, BASE);
            if (arguments->buffer_size <= 0)
            {
              printf("Buffer size must be positive\n");
              return false;
            }
            break;
          default:
            printf("Index %d is out of options\n", option_index);
        }
      }
      break;

      case '?':
        printf("Unknown argument\n");
        break;
      default:
        printf("getopt returned character code 0%o?\n", option_char);
    }
  }

  if (arguments->ip == NULL || arguments->port == -1)
  {
    printf("Using: %s --ip <address> --port <num> [--bufsize <size>]\n",
           *argv[0]);
    printf("Default buffer size: %d\n", DEFAULT_BUFSIZE);
    return false;
  }

  return true;
}

void free_program_arguments(ProgramArguments *arguments)
{
  if (arguments->ip != NULL)
  {
    free(arguments->ip);
    arguments->ip = NULL;
  }
}
