#include "user_service.h"
#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include <time.h>
#include <stdlib.h>



char* get_random_token(size_t length) {

    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";        
    char *randomString = NULL;

    if (length) {
        randomString = malloc(sizeof(char) * (length +1));

        if (randomString) {            
            for (int n = 0;n < length;n++) {            
                int key = rand() % (int)(sizeof(charset) -1);
                randomString[n] = charset[key];
            }

            randomString[length] = '\0';
        }
    }

    return randomString;
}

int get_user_id(char *token)
{
    if (token == NULL)
    {
        return 0;
    }
    int rc;
    sqlite3 *db;
    rc = sqlite3_open("chat-db.db", &db);
    sqlite3_stmt *stmt;


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

    char *sql = "SELECT COUNT(*) FROM users WHERE username LIKE ?";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    int count;
    if(stmt != NULL){
        sqlite3_bind_text(stmt, 1, username->valuestring, -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        count = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
    }
    if(count == 0){

        sqlite3_stmt *stmt2;
        char *sql2 = "INSERT INTO users(username, password) VALUES(?, ?)";
        rc = sqlite3_prepare_v2(db, sql2, -1, &stmt2, 0);

        
        if(stmt2 != NULL) {
            sqlite3_bind_text(stmt2, 1, username->valuestring, -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt2, 2, password->valuestring, -1, SQLITE_TRANSIENT);
            sqlite3_step(stmt2);
            sqlite3_finalize(stmt2);
        }

        sqlite3_stmt *stmt3;
        char *sql3 = "SELECT user_id FROM users WHERE username LIKE ?";
        rc = sqlite3_prepare_v2(db, sql3, -1, &stmt3, 0);

        int user_id;
        if(stmt3 != NULL) {
            sqlite3_bind_text(stmt3, 1, username->valuestring, -1, SQLITE_TRANSIENT);
            sqlite3_step(stmt3);
            user_id = sqlite3_column_int(stmt3, 0);
            sqlite3_finalize(stmt3);
        }



        cJSON *response_json = cJSON_CreateObject();
        cJSON_AddNumberToObject(response_json, "id", 1);
        cJSON_AddStringToObject(response_json, "username", username->valuestring);
        *response_status = HTTP_OK;
        return response_json;
    }
    else{
        cJSON *response_json = cJSON_CreateObject();
        cJSON_AddStringToObject(response_json, "err_msg", "Username taken");
        *response_status = HTTP_BAD_REQUEST;
        return response_json;
    }
}

cJSON *login(cJSON *request, int user_id, http_status *response_status)
{
    cJSON *username = cJSON_GetObjectItemCaseSensitive(request, "username");
    cJSON *password = cJSON_GetObjectItemCaseSensitive(request, "password");


    int rc;
    sqlite3 *db;
    rc = sqlite3_open("chat-db.db", &db);
    sqlite3_stmt *stmt;

    char *sql = "SELECT COUNT(*) FROM users WHERE username LIKE ? AND password LIKE ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    int count;
    if(stmt != NULL){
        sqlite3_bind_text(stmt, 1, username->valuestring, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, password->valuestring, -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        count = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
    }
    if(count == 0){
        cJSON *response_json = cJSON_CreateObject();
        cJSON_AddStringToObject(response_json, "err_msg", "Wrong username or password");
        *response_status = HTTP_BAD_REQUEST;
        return response_json;
    }
    
    
    char token[51];
    strcpy(token, get_random_token(50));
    printf("%s", token);
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
                printf("1");
        }
        printf("end");
        sqlite3_finalize(stmt);
    }
    else{
        printf("Empty");    
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

cJSON *add_message(cJSON *request)
{
    cJSON *from = cJSON_GetObjectItemCaseSensitive(request, "from");
    cJSON *to = cJSON_GetObjectItemCaseSensitive(request, "to");
    cJSON *message = cJSON_GetObjectItemCaseSensitive(request, "message");
    cJSON *timestamp = cJSON_GetObjectItemCaseSensitive(request, "timestamp");
    int user_id = from->valueint;
    int friend_id = to->valueint;
    char *message_str = message->valuestring;
    char *timestamp_str = timestamp->valuestring;
    printf("Add message %s at %s from %d to %d", message_str, timestamp_str, user_id, friend_id);

    cJSON *message_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(message_json, "id", 8);
    cJSON_AddNumberToObject(message_json, "from", user_id);
    cJSON_AddNumberToObject(message_json, "to", friend_id);
    cJSON_AddStringToObject(message_json, "message", message_str);
    cJSON_AddStringToObject(message_json, "timestamp", timestamp_str);
    return message_json;
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
    cJSON_AddNumberToObject(message_json, "id", 1);
    cJSON_AddNumberToObject(message_json, "from", 1);
    cJSON_AddNumberToObject(message_json, "to", 3);
    cJSON_AddStringToObject(message_json, "message", "Siema");
    cJSON_AddStringToObject(message_json, "timestamp", "2022-12-19T15:45:33");
    cJSON_AddItemToArray(response_json, message_json);
    message_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(message_json, "id", 2);
    cJSON_AddNumberToObject(message_json, "from", 3);
    cJSON_AddNumberToObject(message_json, "to", 1);
    cJSON_AddStringToObject(message_json, "message", "Elo");
    cJSON_AddStringToObject(message_json, "timestamp", "2022-12-19T15:52:41");
    cJSON_AddItemToArray(response_json, message_json);
    *response_status = HTTP_OK;
    return response_json;
}
