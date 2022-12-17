#include "requests.h"
#include "picohttpparser.h"
#include "cJSON.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>

request *get_request(int socket)
{
    request *result = malloc(sizeof(request));

    const char *path, *method;
    int pret, minor_version, buf_size = 1024;
    struct phr_header headers[100];
    size_t buflen = 0, prevbuflen = 0, method_len, path_len, num_headers;
    ssize_t rret;
    char *buf = malloc(buf_size * sizeof(char));
    // memset(buf, '\0', buf_size * sizeof(char));
    while (1)
    {
        while ((rret = read(socket, buf + buflen, buf_size - buflen)) == -1 && errno == EINTR)
            ;
        if (rret <= 0)
        {
            printf("Problem with connection");
            return NULL;
        }
        prevbuflen = buflen;
        buflen += rret;
        num_headers = sizeof(headers) / sizeof(headers[0]);
        pret = phr_parse_request(buf, buflen, &method, &method_len, &path, &path_len,
                                 &minor_version, headers, &num_headers, prevbuflen);
        if (pret > 0)
        {
            break;
        }
        else if (pret == -1)
        {
            printf("Problem with parsing");
            free(buf);
            return NULL;
        }
        assert(pret == -2);
        if (buflen == sizeof(buf))
        {
            buf_size *= 2;
            buf = realloc(buf, buf_size);
            // memset(buf, '\0', buf_size * sizeof(char));
        }
    }

    result->path = malloc(path_len + 1);
    strncpy(result->path, path, path_len);
    result->path[path_len] = '\0';
    result->method = malloc(method_len + 1);
    strncpy(result->method, method, method_len);
    result->method[method_len] = '\0';
    result->has_auth = false;
    for (int i = 0; i < num_headers; i++)
    {
        if (strncmp("Authorization", headers[i].name, headers[i].name_len) == 0)
        {
            result->auth = malloc(headers[i].value_len + 1);
            strncpy(result->auth, headers[i].value, headers[i].value_len);
            result->method[headers[i].value_len] = '\0';
            result->has_auth = true;
        }
    }
    result->has_data = false;
    char *data_ptr = strstr(buf, "\r\n\r\n");
    if (data_ptr != NULL)
    {
        char *body_ptr = data_ptr + 4;
        if (strlen(body_ptr) > 0)
        {
            result->data = malloc(strlen(body_ptr) + 1);
            strncpy(result->data, body_ptr, strlen(body_ptr));
            result->data[strlen(body_ptr)] = '\0';
            result->has_data = true;
        }
    }
    free(buf);
    return result;
}

void destroy_request(request *request)
{
    free(request->path);
    free(request->method);
    if (request->has_auth == true)
    {
        free(request->auth);
    }
    if (request->has_data == true)
    {
        free(request->data);
    }
    free(request);
}

char *get_resposne(http_status status, cJSON *payload)
{
    char *response;
    char *body = malloc(sizeof(char));
    body[0] = '\0';
    if (payload != NULL)
    {
        free(body);
        body = cJSON_Print(payload);
        response = malloc(sizeof(char) * (160 + strlen(body)));
    }
    else
    {
        response = malloc(sizeof(char) * 160);
    }
    char *status_str;
    if (status == HTTP_OK)
    {
        status_str = "200 OK";
    }
    sprintf(response, "HTTP/1.1 %s\r\nAccess-Control-Allow-Origin: http://localhost:3000\r\nAccess-Control-Allow-Headers: *\r\nContent-Type: application/json\r\n\r\n%s", status_str, body);
    free(body);
    return response;
}