#include <getopt.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "server_arguments.h"
#include "string_converter.h"

bool parseArguments(int *argc, char ***argv, ProgramArguments *arguments) {
  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"port", required_argument, 0, 0},
                                      {"tnum", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(*argc, *argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        arguments->port = atoi(optarg);
        break;
      case 1:
        arguments->tnum = atoi(optarg);
        break;
      case 2:
        ConvertStringToUI64(optarg, &arguments->mod);
        break;
      default:
        printf("Index %d is out of options\n", option_index);
      }
    } break;

    case '?':
      printf("Unknown argument\n");
      break;
    default:
      printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (arguments->port == -1 || arguments->tnum == -1 || arguments->mod == -1) {
    printf("Using: %s --port <num> --tnum <num> --mod <num>\n", *argv[0]);
    return false;
  }

  return true;
}
