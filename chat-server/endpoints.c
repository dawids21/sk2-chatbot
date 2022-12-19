#include "endpoints.h"

cJSON *get_null(cJSON *request)
{
    return NULL;
}

cJSON *get_hello(cJSON *request)
{
    cJSON *response_json = cJSON_CreateObject();
    cJSON_AddStringToObject(response_json, "response", "Hello, World!");
    return response_json;
}