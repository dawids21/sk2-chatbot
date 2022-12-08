#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>  // for open
#include <unistd.h> // for close
#include <pthread.h>
#include "cJSON.h"
#include "requests.h"

char buffer[1024];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *socketThread(void *arg)
{
  printf("new thread\n");
  char *client_message = malloc(sizeof(char) * 2000);
  int newSocket = *((int *)arg);
  int n = recv(newSocket, client_message, 2000, 0);
  printf("%s\n", client_message);
  if (n < 1)
  {
    pthread_exit(NULL);
  }
  request request = parse_request(client_message, n);
  free(client_message);

  cJSON *json = cJSON_CreateObject();
  cJSON_AddStringToObject(json, "data", "data2");

  char *response = get_resposne(HTTP_OK, json);
  send(newSocket, response, strlen(response), 0);
  close(newSocket);
  destroy_request(request);
  cJSON_Delete(json);
  free(response);
  printf("Exit socketThread \n");
  pthread_exit(NULL);
}

int main()
{
  int serverSocket, newSocket;
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  serverSocket = socket(PF_INET, SOCK_STREAM, 0);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(1100);
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

  if (listen(serverSocket, 50) == 0)
    printf("Listening\n");
  else
    printf("Error\n");
  pthread_t thread_id;

  while (1)
  {
    addr_size = sizeof serverStorage;
    newSocket = accept(serverSocket, (struct sockaddr *)&serverStorage, &addr_size);
    if (pthread_create(&thread_id, NULL, socketThread, &newSocket) != 0)
      printf("Failed to create thread\n");
    pthread_detach(thread_id);
  }
  return 0;
}
