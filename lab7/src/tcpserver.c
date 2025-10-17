#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../libs/tcpserver_arguments.h"

#define SADDR struct sockaddr

int main(int argc, char **argv)
{
  ProgramArguments args;

  if (!parse_arguments(&argc, &argv, &args))
  {
    return 1;
  }

  const size_t address_size = sizeof(struct sockaddr_in);

  int listen_descriptor;
  int client_descriptor;
  int bytes_read;
  char *buffer = (char *)malloc(args.buffer_size);
  if (buffer == NULL)
  {
    printf("Произошла ошибка при выделении памяти!\n");
    return 1;
  }

  struct sockaddr_in server_address;
  struct sockaddr_in client_address;

  listen_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_descriptor < 0)
  {
    printf("Произошла ошибка при создании прослушивающего сокета!\n");
    free(buffer);
    exit(1);
  }

  memset(&server_address, 0, address_size);
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(args.port);

  if (bind(listen_descriptor, (SADDR *)&server_address, address_size) < 0)
  {
    printf(
      "Произошла ошибка при привязке послушивающего сокета к адресу "
      "сервера!\n");
    free(buffer);
    close(listen_descriptor);
    exit(1);
  }

  if (listen(listen_descriptor, args.backlog) < 0)
  {
    printf("Было превышено максимальное количество доступных соединений!\n");
    free(buffer);
    close(listen_descriptor);
    exit(1);
  }

  while (true)
  {
    unsigned int clilen = address_size;

    client_descriptor =
      accept(listen_descriptor, (SADDR *)&client_address, &clilen);
    if (client_descriptor < 0)
    {
      printf("Произошла ошибка при установке соединения с клиентом!\n");
      continue;
    }

    printf("Соединение с клиентом успешно установлено!\n");

    while (
      (bytes_read = (int)read(client_descriptor, buffer, args.buffer_size)) > 0)
    {
      printf("Сообщение от клиента :");
      write(STDOUT_FILENO, buffer, bytes_read);

      if (write(client_descriptor, buffer, bytes_read) < 0)
      {
        printf("Произошла ошибка при отправке ответа клиенту!\n");
        break;
      }
    }

    if (bytes_read == -1)
    {
      printf("Произошла ошибка при чтении сообщения от клиента!\n");
      close(client_descriptor);
    }

    free(buffer);
    close(client_descriptor);
    close(listen_descriptor);
  }
}
