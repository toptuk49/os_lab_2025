#include <arpa/inet.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../libs/client_arguments.h"

#define SADDR struct sockaddr
#define SIZE sizeof(struct sockaddr_in)
#define PROMPT_MESSAGE "Введите сообщение, которое хотите отправить:\n"

int main(int argc, char *argv[])
{
  ProgramArguments args;

  if (!parse_arguments(&argc, &argv, &args))
  {
    return 1;
  }

  int socket_descriptor;
  int bytes_read;
  int bytes_received;
  char *send_buffer = (char *)malloc(args.buffer_size);
  char *receive_buffer = (char *)malloc(args.buffer_size);
  if (send_buffer == NULL || receive_buffer == NULL)
  {
    printf("Произошла ошибка при выделении памяти!\n");
    free_program_arguments(&args);
    free(send_buffer);
    free(receive_buffer);
    return 1;
  }

  struct sockaddr_in server_address;

  socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_descriptor < 0)
  {
    printf("Произошла ошибка при создании сокета!\n");
    free_program_arguments(&args);
    free(send_buffer);
    free(receive_buffer);
    exit(1);
  }

  memset(&server_address, 0, SIZE);
  server_address.sin_family = AF_INET;

  if (inet_pton(AF_INET, args.ip, &server_address.sin_addr) <= 0)
  {
    printf("Введен неверный ip адрес!\n");
    free_program_arguments(&args);
    free(send_buffer);
    free(receive_buffer);
    close(socket_descriptor);
    exit(1);
  }

  server_address.sin_port = htons((uint16_t)args.port);

  if (connect(socket_descriptor, (SADDR *)&server_address, SIZE) < 0)
  {
    printf("Произошла ошибка при попытке соединения с сервером!\n");
    free_program_arguments(&args);
    free(send_buffer);
    free(receive_buffer);
    close(socket_descriptor);
    exit(1);
  }

  write(STDOUT_FILENO, PROMPT_MESSAGE, strlen(PROMPT_MESSAGE));
  while ((bytes_read = (int)read(0, send_buffer, args.buffer_size)) > 0)
  {
    if (write(socket_descriptor, send_buffer, bytes_read) < 0)
    {
      printf("Произошла ошибка при отправке сообщения на сервер!\n");
      free_program_arguments(&args);
      free(send_buffer);
      free(receive_buffer);
      close(socket_descriptor);
      exit(1);
    }

    bytes_received =
      (int)read(socket_descriptor, receive_buffer, args.buffer_size - 1);
    if (bytes_received < 0)
    {
      printf("Произошла ошибка при получении ответа от сервера!\n");
      free_program_arguments(&args);
      free(send_buffer);
      free(receive_buffer);
      close(socket_descriptor);
      exit(1);
    }

    if (bytes_received == 0)
    {
      printf("Сервер закрыл соединение!\n");
      break;
    }

    receive_buffer[bytes_received] = '\0';
    printf("Ответ от сервера: %s\n", receive_buffer);
  }

  free_program_arguments(&args);
  free(send_buffer);
  free(receive_buffer);
  close(socket_descriptor);
  exit(0);
}
