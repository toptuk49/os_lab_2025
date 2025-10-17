#include <arpa/inet.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../libs/client_arguments.h"

#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)
#define PROMPT_MESSAGE "Введите сообщение, которое хотите отправить:\n"

int main(int argc, char **argv)
{
  ProgramArguments args;

  if (!parse_arguments(&argc, &argv, &args))
  {
    return 1;
  }

  int socket_descriptor;
  int bytes_read;
  char *line_to_send = (char *)malloc(args.buffer_size);
  char *line_to_receive = (char *)malloc(args.buffer_size + 1);
  if (line_to_send == NULL || line_to_receive == NULL)
  {
    printf("Произошла ошибка при выделении памяти!\n");
    free_program_arguments(&args);
    free(line_to_send);
    free(line_to_receive);
    return 1;
  }

  struct sockaddr_in server_address;
  struct sockaddr_in client_address;

  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons((uint16_t)args.port);

  if (inet_pton(AF_INET, args.ip, &server_address.sin_addr) < 0)
  {
    printf("Произошла ошибка при преобразовании IP адреса!\n");
    free_program_arguments(&args);
    free(line_to_send);
    free(line_to_receive);
    exit(1);
  }

  socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_descriptor < 0)
  {
    printf("Произошла ошибка при создании сокета!\n");
    free_program_arguments(&args);
    free(line_to_send);
    free(line_to_receive);
    exit(1);
  }

  write(STDOUT_FILENO, PROMPT_MESSAGE, strlen(PROMPT_MESSAGE));

  while ((bytes_read = (int)read(0, line_to_send, args.buffer_size)) > 0)
  {
    if (sendto(socket_descriptor, line_to_send, bytes_read, 0,
               (SADDR *)&server_address, SLEN) == -1)
    {
      printf("Произошла ошибка при отправке сообщения на сервер!\n");
      free_program_arguments(&args);
      free(line_to_send);
      free(line_to_receive);
      close(socket_descriptor);
      exit(1);
    }

    memset(line_to_receive, 0, args.buffer_size + 1);

    if (recvfrom(socket_descriptor, line_to_receive, args.buffer_size, 0, NULL,
                 NULL) == -1)
    {
      printf("Произошла ошибка при получении сообщения с сервера!\n");
      free_program_arguments(&args);
      free(line_to_send);
      free(line_to_receive);
      close(socket_descriptor);
      exit(1);
    }

    printf("Ответ сервера: %s\n", line_to_receive);
  }

  free_program_arguments(&args);
  free(line_to_send);
  free(line_to_receive);
  close(socket_descriptor);
  return 0;
}
