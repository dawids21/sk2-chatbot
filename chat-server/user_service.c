#include "user_service.h"
#include <stdio.h>
#include <string.h>
#include <sqlite3.h>


// char* gen_random() {
//     static const char alphanum[] =
//         "0123456789"
//         "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
//         "abcdefghijklmnopqrstuvwxyz";
//     char tmp_s[50];

//     for (int i = 0; i < 50; ++i) {
//         tmp_s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
//     }
    
//     return tmp_s;
// }

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
    int rc;
    sqlite3 *db;
    rc = sqlite3_open("chat-db.db", &db);
    sqlite3_stmt *stmt;

    cJSON *username = cJSON_GetObjectItemCaseSensitive(request, "username");
    cJSON *password = cJSON_GetObjectItemCaseSensitive(request, "password");

    char *sql = "INSERT INTO users(username, password) VALUES(?, ?)";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    
    if(stmt != NULL) {
        sqlite3_bind_text(stmt, 1, username->valuestring, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, password->valuestring, -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    cJSON *response_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(response_json, "id", 1);  //todo pobrac id
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
    // if (user_id == 0)
    // {
    //     *response_status = HTTP_UNAUTHORIZED;
    //     return NULL;
    // }

    int rc;
    sqlite3 *db;
    rc = sqlite3_open("chat-db.db", &db);
    sqlite3_stmt *stmt;

    char *sql = "SELECT user_id, username FROM users";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    cJSON *response_json = cJSON_CreateArray();
    if(stmt != NULL){
            while (sqlite3_step(stmt) != SQLITE_DONE){
                cJSON *user_json = cJSON_CreateObject();
                cJSON_AddNumberToObject(user_json, "user_id", sqlite3_column_int(stmt, 0));
                cJSON_AddStringToObject(user_json, "username", sqlite3_column_text(stmt, 1));
                cJSON_AddItemToArray(response_json, user_json);
            }
    }
    sqlite3_finalize(stmt);
    *response_status = HTTP_OK;
    return response_json;
}

cJSON *get_users_by_username(cJSON *request, int user_id, http_status *response_status)
{
    // if (user_id == 0)
    // {
    //     *response_status = HTTP_UNAUTHORIZED;
    //     return NULL;
    // }

    int rc;
    sqlite3 *db;
    rc = sqlite3_open("chat-db.db", &db);
    sqlite3_stmt *stmt;

    cJSON *username = cJSON_GetObjectItemCaseSensitive(request, "username");

    char *sql = "SELECT user_id, username FROM users WHERE username LIKE ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    cJSON *response_json = cJSON_CreateArray();
    if(stmt != NULL) {
        sqlite3_bind_text(stmt, 1, username->valuestring, -1, SQLITE_TRANSIENT);

        while (sqlite3_step(stmt) != SQLITE_DONE){
                cJSON *user_json = cJSON_CreateObject();
                cJSON_AddNumberToObject(user_json, "user_id", sqlite3_column_int(stmt, 0));
                cJSON_AddStringToObject(user_json, "username", sqlite3_column_text(stmt, 1));
                cJSON_AddItemToArray(response_json, user_json);
        }
        sqlite3_finalize(stmt);
    }
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

    int rc;
    sqlite3 *db;
    rc = sqlite3_open("chat-db.db", &db);
    sqlite3_stmt *stmt;

    cJSON *friend_id = cJSON_GetObjectItemCaseSensitive(request, "friend_id");
    char *sql = "INSERT INTO friends VALUES(?, ?)";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if(stmt != NULL) {
        sqlite3_bind_int(stmt, 1, user_id);
        sqlite3_bind_int(stmt, 2, friend_id);
        sqlite3_step(stmt);
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if(stmt != NULL){
        sqlite3_bind_int(stmt, 2, user_id);
        sqlite3_bind_int(stmt, 1, friend_id);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

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