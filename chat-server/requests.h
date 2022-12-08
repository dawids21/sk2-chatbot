#ifndef REQUESTS_H
#define REQUESTS_H

#include <stdbool.h>
#include "cJSON.h"

typedef int http_status;

#define HTTP_OK ((http_status)1)

typedef struct
{
    char *path;
    char *method;
    char *auth;
    bool has_auth;
    char *data;
    bool has_data;
} request;

request parse_request(char *raw, int length);
void destroy_request(request request);
char *get_resposne(http_status status, cJSON *payload);

#endif