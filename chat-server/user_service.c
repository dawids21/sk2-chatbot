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
    cJSON_AddNumberToObject(user_json, "id", 1);
    cJSON_AddStringToObject(user_json, "username", "dawids21");
    cJSON_AddItemToArray(response_json, user_json);
    user_json = cJSON_CreateObject();
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

cJSON *get_users_by_username(cJSON *request, int user_id, http_status *response_status)
{
    if (user_id == 0)
    {
        *response_status = HTTP_UNAUTHORIZED;
        return NULL;
    }

    cJSON *username = cJSON_GetObjectItemCaseSensitive(request, "username");
    printf("Find by username: %s\n", username->valuestring);

    cJSON *response_json = cJSON_CreateArray();
    cJSON *user_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(user_json, "id", 2);
    cJSON_AddStringToObject(user_json, "username", "jan");
    cJSON_AddItemToArray(response_json, user_json);
    *response_status = HTTP_OK;
    return response_json;
}

cJSON *add_friend(cJSON *request, int user_id, http_status *response_status)
{

    if (user_id == 0)
    {
        *response_status = HTTP_UNAUTHORIZED;
        return NULL;
    }

    cJSON *friend_id = cJSON_GetObjectItemCaseSensitive(request, "friend_id");
    printf("Add friend: %d\n", friend_id->valueint);

    *response_status = HTTP_OK;
    return NULL;
}

cJSON *delete_friend(cJSON *request, int user_id, http_status *response_status)
{
    if (user_id == 0)
    {
        *response_status = HTTP_UNAUTHORIZED;
        return NULL;
    }

    cJSON *friend_id = cJSON_GetObjectItemCaseSensitive(request, "friend_id");
    printf("Delete friend: %d\n", friend_id->valueint);

    *response_status = HTTP_OK;
    return NULL;
}

cJSON *get_friends(cJSON *request, int user_id, http_status *response_status)
{
    if (user_id == 0)
    {
        *response_status = HTTP_UNAUTHORIZED;
        return NULL;
    }

    printf("Get friends\n");

    cJSON *response_json = cJSON_CreateArray();
    cJSON *user_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(user_json, "id", 3);
    cJSON_AddStringToObject(user_json, "username", "adam");
    cJSON_AddItemToArray(response_json, user_json);
    *response_status = HTTP_OK;
    return response_json;
}

void add_message(int user_id, int friend_id, char *message)
{
    // TODO nie wiem jak to jeszcze będzie wyglądać, póki nie ma web socketów
    printf("Add message %s from %d to %d", message, user_id, friend_id);
}

cJSON *get_messages(cJSON *request, int user_id, http_status *response_status)
{
    if (user_id == 0)
    {
        *response_status = HTTP_UNAUTHORIZED;
        return NULL;
    }

    cJSON *friend_id = cJSON_GetObjectItemCaseSensitive(request, "friend_id");
    printf("Get messages with friend %d\n", friend_id->valueint);

    cJSON *response_json = cJSON_CreateArray();
    cJSON *message_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(message_json, "user_id", 1);
    cJSON_AddStringToObject(message_json, "message", "Siema");
    cJSON_AddStringToObject(message_json, "timestamp", "2022-12-19T15:45:33");
    cJSON_AddItemToArray(response_json, message_json);
    message_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(message_json, "user_id", 3);
    cJSON_AddStringToObject(message_json, "message", "Elo");
    cJSON_AddStringToObject(message_json, "timestamp", "2022-12-19T15:52:41");
    cJSON_AddItemToArray(response_json, message_json);
    *response_status = HTTP_OK;
    return response_json;
}