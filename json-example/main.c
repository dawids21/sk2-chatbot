#include <stdio.h>
#include <malloc.h>
#include "./cJSON.h"

void parseJson()
{
    char json[] = "{\"operation\": \"/login\", \"username\": \"dawid\", \"password\": \"stasiak\"}";
    cJSON *data = cJSON_Parse(json);
    cJSON *operation = cJSON_GetObjectItemCaseSensitive(data, "operation");
    cJSON *username = cJSON_GetObjectItemCaseSensitive(data, "username");
    cJSON *password = cJSON_GetObjectItemCaseSensitive(data, "password");
    printf("Operation: %s, Username: %s, Password: %s\n", operation->valuestring, username->valuestring, password->valuestring);
    cJSON_Delete(data);
}

void createJson()
{
    char token[] = "1234567890";
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "token", token);
    char *jsonString = cJSON_Print(json);
    printf("%s\n", jsonString);
    cJSON_Delete(json);
    free(jsonString);
}

int main(int argc, char const *argv[])
{
    parseJson();
    createJson();
    return 0;
}
