#include "tcpserver_arguments.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#define BASE 10
#define DEFAULT_PORT 10050
#define DEFAULT_BUFSIZE 100
#define DEFAULT_BACKLOG 5
#define MAX_PORT 65536

bool parse_arguments(int *argc, char ***argv, ProgramArguments *arguments)
{
  arguments->port = DEFAULT_PORT;
  arguments->buffer_size = DEFAULT_BUFSIZE;
  arguments->backlog = DEFAULT_BACKLOG;

  while (true)
  {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"port", required_argument, 0, 0},
                                      {"bufsize", required_argument, 0, 0},
                                      {"backlog", required_argument, 0, 0},
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
            arguments->port = (int)strtol(optarg, NULL, BASE);
            if (arguments->port <= 0 || arguments->port > MAX_PORT)
            {
              printf("Port must be in range 1-65535\n");
              return false;
            }
            break;
          case 1:
            arguments->buffer_size = (int)strtol(optarg, NULL, BASE);
            if (arguments->buffer_size <= 0)
            {
              printf("Buffer size must be positive\n");
              return false;
            }
            break;
          case 2:
            arguments->backlog = (int)strtol(optarg, NULL, BASE);
            if (arguments->backlog <= 0)
            {
              printf("Backlog must be positive\n");
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

  return true;
}
