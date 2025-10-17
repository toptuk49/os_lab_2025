#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../libs/udpserver_arguments.h"

#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)
#define IPV4_LENGTH 16

int main(int argc, char **argv)
{
  ProgramArguments args;

  if (!parse_arguments(&argc, &argv, &args))
  {
    return 1;
  }

  int socket_descriptor;
  int bytes_read;
  char *message = (char *)malloc(args.buffer_size);
  if (message == NULL)
  {
    printf("Произошла ошибка при выделении памяти!\n");
    return 1;
  }

  char ip_address[IPV4_LENGTH];
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;

  socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_descriptor < 0)
  {
    printf("Произошла ошибка при создании сокета!\n");
    free(message);
    exit(1);
  }

  memset(&server_address, 0, SLEN);
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(args.port);

  if (bind(socket_descriptor, (SADDR *)&server_address, SLEN) < 0)
  {
    printf(
      "Произошла ошибка при привязке послушивающего сокета к адресу "
      "сервера!\n");
    free(message);
    close(socket_descriptor);
    exit(1);
  }
  printf("Сервер запускается на порту %d...\n", args.port);

  while (true)
  {
    unsigned int len = SLEN;

    bytes_read = (int)recvfrom(socket_descriptor, message, args.buffer_size, 0,
                               (SADDR *)&client_address, &len);
    if (bytes_read < 0)
    {
      printf("Произошла ошибка при получении сообщения от клиента!\n");
      continue;
    }
    message[bytes_read] = 0;

    printf("СООБЩЕНИЕ %s      ОТ %s : %d\n", message,
           inet_ntop(AF_INET, (void *)&client_address.sin_addr.s_addr,
                     ip_address, IPV4_LENGTH),
           ntohs(client_address.sin_port));

    if (sendto(socket_descriptor, message, bytes_read, 0,
               (SADDR *)&client_address, len) < 0)
    {
      printf("Произошла ошибка при отправке сообщения клиенту!\n");
      continue;
    }
  }

  free(message);
  close(socket_descriptor);
  return 0;
}
