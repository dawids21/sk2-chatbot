#include "user_service.h"
#include <stdio.h>
#include <string.h>

int get_user_id(char *token)
{
    if (token == NULL)
    {
        return 0;
    }
    if (strcmp("12345", token) == 0)
    {
        return 1;
    }
    return 0;
}

cJSON *register_user(cJSON *request, int user_id, http_status *response_status)
{
    cJSON *username = cJSON_GetObjectItemCaseSensitive(request, "username");
    cJSON *password = cJSON_GetObjectItemCaseSensitive(request, "password");

    printf("Username: %s, password: %s\n", username->valuestring, password->valuestring);

    cJSON *response_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(response_json, "id", 1);
    cJSON_AddStringToObject(response_json, "username", username->valuestring);
    *response_status = HTTP_OK;
    return response_json;
}

cJSON *login(cJSON *request, int user_id, http_status *response_status)
{
    cJSON *username = cJSON_GetObjectItemCaseSensitive(request, "username");
    cJSON *password = cJSON_GetObjectItemCaseSensitive(request, "password");

    printf("Username: %s, password: %s\n", username->valuestring, password->valuestring);

    cJSON *response_json = cJSON_CreateObject();
    cJSON_AddStringToObject(response_json, "token", "12345");
    *response_status = HTTP_OK;
    return response_json;
}

cJSON *get_users(cJSON *request, int user_id, http_status *response_status)
{
    if (user_id == 0)
    {
        *response_status = HTTP_UNAUTHORIZED;
        return NULL;
    }
    cJSON *response_json = cJSON_CreateArray();
    cJSON *user_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(user_json, "id", 2);
    cJSON_AddStringToObject(user_json, "username", "jan");
    cJSON_AddItemToArray(response_json, user_json);
    user_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(user_json, "id", 3);
    cJSON_AddStringToObject(user_json, "username", "adam");
    cJSON_AddItemToArray(response_json, user_json);
    *response_status = HTTP_OK;
    return response_json;
}