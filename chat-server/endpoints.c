#include "endpoints.h"
#include "user_service.h"

cJSON *get_null(cJSON *request, http_status *response_status)
{
    *response_status = HTTP_BAD_REQUEST;
    return NULL;
}

cJSON *get_hello(cJSON *request, http_status *response_status)
{
    cJSON *response_json = cJSON_CreateObject();
    cJSON_AddStringToObject(response_json, "response", "Hello, World!");
    *response_status = HTTP_OK;
    return response_json;
}

cJSON *post_register(cJSON *request, http_status *response_status)
{
    cJSON *username = cJSON_GetObjectItemCaseSensitive(request, "username");
    cJSON *password = cJSON_GetObjectItemCaseSensitive(request, "password");
    register_user(username->valuestring, password->valuestring);
    *response_status = HTTP_OK;
    return NULL;
}