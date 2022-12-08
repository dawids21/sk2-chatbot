#include "requests.h"
#include "picohttpparser.h"
#include "cJSON.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

request parse_request(char *raw, int length)
{
    request request;
    const char *method, *path;
    int minor_version;
    struct phr_header headers[100];
    size_t method_len, path_len, num_headers;
    num_headers = sizeof(headers) / sizeof(headers[0]);
    phr_parse_request(raw, length, &method, &method_len, &path, &path_len,
                      &minor_version, headers, &num_headers, 0);
    request.path = malloc(path_len + 1);
    strncpy(request.path, path, path_len);
    request.path[path_len] = '\0';
    request.method = malloc(method_len + 1);
    strncpy(request.method, method, method_len);
    request.method[method_len] = '\0';
    request.has_auth = false;
    for (int i = 0; i < num_headers; i++)
    {
        if (strncmp("authorization", headers[i].name, headers[i].name_len) == 0)
        {
            request.auth = malloc(headers[i].value_len + 1);
            strncpy(request.auth, headers[i].value, headers[i].value_len);
            request.method[headers[i].value_len] = '\0';
            request.has_auth = true;
        }
    }
    request.has_data = false;
    char *data_ptr = strstr(raw, "\r\n\r\n");
    if (data_ptr != NULL)
    {
        char *body_ptr = data_ptr + 4;
        if (strlen(body_ptr) > 0)
        {
            request.data = malloc(strlen(body_ptr) + 1);
            strncpy(request.data, body_ptr, strlen(body_ptr));
            request.data[strlen(body_ptr)] = '\0';
            request.has_data = true;
        }
    }
    return request;
}

void destroy_request(request request)
{
    free(request.path);
    free(request.method);
    if (request.has_auth == true)
    {
        free(request.auth);
    }
    if (request.has_data == true)
    {
        free(request.data);
    }
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
        response = malloc(sizeof(char) * (120 + strlen(body)));
    }
    else
    {
        response = malloc(sizeof(char) * 120);
    }
    char *status_str;
    if (status == HTTP_OK)
    {
        status_str = "200 OK";
    }
    sprintf(response, "HTTP/1.1 %s\r\nAccess-Control-Allow-Origin: http://localhost:3000\r\nContent-Type: application/json\r\n\r\n%s", status_str, body);
    free(body);
    return response;
}