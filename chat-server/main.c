#include "requests.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>

static int serverSocket;

void int_handler(int sign)
{
    shutdown(serverSocket, SHUT_RD);
    close(serverSocket);
}

bool is_operation(request *request, char *method, char *path)
{
    if (strcmp(request->method, method) == 0 && strcmp(request->path, path) == 0)
    {
        return true;
    }
    return false;
}

char *handle_request(request *request)
{
    cJSON *request_json = NULL;
    if (request->has_data)
    {
        request_json = cJSON_Parse(request->data);
    }

    char *response = NULL;
    cJSON *response_json = NULL;
    if (strcmp(request->method, "OPTIONS") == 0)
    {
        response = get_resposne(HTTP_OK, NULL);
    }
    else if (is_operation(request, "POST", "/hello"))
    {
        response_json = cJSON_CreateObject();
        cJSON_AddStringToObject(response_json, "response", "Hello, World!");
        response = get_resposne(HTTP_OK, response_json);
    }
    else
    {
        response = get_resposne(HTTP_OK, NULL);
    }
    if (response_json != NULL)
    {
        cJSON_Delete(response_json);
    }

    if (request->has_data)
    {
        cJSON_Delete(request_json);
    }
    return response;
}

void *socketThread(void *arg)
{
    int socket = *((int *)arg);

    request *request = get_request(socket);
    printf("%s\n", request->method);
    printf("%s\n", request->path);
    if (request->has_auth == true)
    {
        printf("%s\n", request->auth);
    }
    if (request->has_data == true)
    {
        printf("%s\n", request->data);
    }

    char *response = handle_request(request);
    send(socket, response, strlen(response), 0);

    close(socket);
    destroy_request(request);
    free(response);

    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, int_handler);
    int newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    const int one = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
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
        if (newSocket == -1)
        {
            break;
        }
        if (pthread_create(&thread_id, NULL, socketThread, &newSocket) != 0)
            printf("Failed to create thread\n");

        pthread_detach(thread_id);
    }
    return 0;
}
