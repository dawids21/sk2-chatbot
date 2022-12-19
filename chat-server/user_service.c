#include "user_service.h"
#include <stdio.h>

cJSON *register_user(cJSON *request, http_status *response_status)
{
    cJSON *username = cJSON_GetObjectItemCaseSensitive(request, "username");
    cJSON *password = cJSON_GetObjectItemCaseSensitive(request, "password");
    printf("Username: %s, password: %s\n", username->valuestring, password->valuestring);
    *response_status = HTTP_OK;
    return NULL;
}