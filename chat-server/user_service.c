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
    sqlite3 *db;
    sqlite3_open("chat-db.db", &db);
    sqlite3_stmt *stmt;

    char *sql = "SELECT user_id FROM logged_in_users WHERE token LIKE ?";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    
    int user_id = 0;
    if(stmt != NULL){
        sqlite3_bind_text(stmt, 1, token, -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        user_id = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);
    return user_id;
}

cJSON *register_user(cJSON *request, int user_id, http_status *response_status)
{
    sqlite3 *db;
    sqlite3_open("chat-db.db", &db);
    sqlite3_stmt *stmt;
    

    cJSON *username = cJSON_GetObjectItemCaseSensitive(request, "username");
    cJSON *password = cJSON_GetObjectItemCaseSensitive(request, "password");

    char *sql = "SELECT COUNT(*) FROM users WHERE username LIKE ?";

    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    int count = 0;
    if(stmt != NULL){
        sqlite3_bind_text(stmt, 1, username->valuestring, -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        count = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
    }
    if(count == 0){

        sqlite3_stmt *stmt2;
        char *sql2 = "INSERT INTO users(username, password) VALUES(?, ?)";
        sqlite3_prepare_v2(db, sql2, -1, &stmt2, 0);

        
        if(stmt2 != NULL) {
            sqlite3_bind_text(stmt2, 1, username->valuestring, -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt2, 2, password->valuestring, -1, SQLITE_TRANSIENT);
            sqlite3_step(stmt2);
            sqlite3_finalize(stmt2);
        }

        sqlite3_stmt *stmt3;
        char *sql3 = "SELECT user_id FROM users WHERE username LIKE ?";
        sqlite3_prepare_v2(db, sql3, -1, &stmt3, 0);

        int user_id = 0;
        if(stmt3 != NULL) {
            sqlite3_bind_text(stmt3, 1, username->valuestring, -1, SQLITE_TRANSIENT);
            sqlite3_step(stmt3);
            user_id = sqlite3_column_int(stmt3, 0);
            sqlite3_finalize(stmt3);
        }



        cJSON *response_json = cJSON_CreateObject();
        cJSON_AddNumberToObject(response_json, "id", user_id);
        cJSON_AddStringToObject(response_json, "username", username->valuestring);

        sqlite3_close(db);
        *response_status = HTTP_OK;
        return response_json;
    }
    else{
        cJSON *response_json = cJSON_CreateObject();
        cJSON_AddStringToObject(response_json, "err_msg", "Username taken");

        sqlite3_close(db);
        *response_status = HTTP_UNAUTHORIZED;
        return response_json;
    }
}

cJSON *login(cJSON *request, int user_id, http_status *response_status)
{
    cJSON *username = cJSON_GetObjectItemCaseSensitive(request, "username");
    cJSON *password = cJSON_GetObjectItemCaseSensitive(request, "password");

    sqlite3 *db;
    sqlite3_open("chat-db.db", &db);
    sqlite3_stmt *stmt;

    char *sql = "SELECT COUNT(*) FROM users WHERE username LIKE ? AND password LIKE ?";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    int count = 0;
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

        sqlite3_close(db);
        *response_status = HTTP_UNAUTHORIZED;
        return response_json;
    }

    sqlite3_stmt *stmt2;
    char *sql2 = "SELECT user_id FROM users WHERE username LIKE ?";
    sqlite3_prepare_v2(db, sql2, -1, &stmt2, 0);
    user_id = 0;

    if(stmt2 != NULL){
        sqlite3_bind_text(stmt2, 1, username->valuestring, -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt2);
        user_id = sqlite3_column_int(stmt2, 0);
        sqlite3_finalize(stmt2);
    }
    if(user_id != 0){
        sqlite3_stmt *stmt3;
        char *sql3 = "DELETE FROM logged_in_users WHERE user_id = ?";
        sqlite3_prepare_v2(db, sql3, -1, &stmt3, 0);
            if(stmt3 != NULL){
                sqlite3_bind_int(stmt3, 1, user_id);
                sqlite3_step(stmt3);
                sqlite3_finalize(stmt3);
            }
    }

    char token[51];
    strcpy(token, get_random_token(50));

    sqlite3_stmt *stmt4;
    char *sql4 = "INSERT INTO logged_in_users VALUES(?, ?);";
    sqlite3_prepare_v2(db, sql4, -1, &stmt4, 0);
    if(stmt4 != NULL){
        sqlite3_bind_int(stmt4, 1, user_id);
        sqlite3_bind_text(stmt4, 2, token, -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt4);
        sqlite3_finalize(stmt4);
    }
   

    cJSON *response_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(response_json, "user_id", user_id);
    cJSON_AddStringToObject(response_json, "token", token);

    sqlite3_close(db);
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

    sqlite3 *db;
    sqlite3_open("chat-db.db", &db);
    sqlite3_stmt *stmt;

    char *sql = "SELECT user_id, username FROM users";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
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

    sqlite3_close(db);
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

    sqlite3 *db;
    sqlite3_open("chat-db.db", &db);
    sqlite3_stmt *stmt;

    cJSON *username = cJSON_GetObjectItemCaseSensitive(request, "username");

    char *sql = "SELECT user_id, username FROM users WHERE username LIKE ?";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
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

    sqlite3_close(db);
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

    sqlite3 *db;
    sqlite3_open("chat-db.db", &db);
    sqlite3_stmt *stmt;

    cJSON *friend_id = cJSON_GetObjectItemCaseSensitive(request, "friend_id");
    char *sql = "INSERT INTO friends VALUES(?, ?)";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if(stmt != NULL) {
        sqlite3_bind_int(stmt, 1, user_id);
        sqlite3_bind_int(stmt, 2, friend_id->valueint);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if(stmt != NULL){
        sqlite3_bind_int(stmt, 2, user_id);
        sqlite3_bind_int(stmt, 1, friend_id->valueint);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);
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

    sqlite3 *db;
    sqlite3_open("chat-db.db", &db);
    sqlite3_stmt *stmt;

    cJSON *friend_id = cJSON_GetObjectItemCaseSensitive(request, "friend_id");
    char *sql = "DELETE FROM friends WHERE (user1_id = ? AND user2_id = ?) OR (user1_id = ? AND user2_id = ?)";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if(stmt != NULL) {
        sqlite3_bind_int(stmt, 1, user_id);
        sqlite3_bind_int(stmt, 2, friend_id->valueint);
        sqlite3_bind_int(stmt, 4, user_id);
        sqlite3_bind_int(stmt, 3, friend_id->valueint);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    sqlite3_close(db);
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
    sqlite3 *db;
    sqlite3_open("chat-db.db", &db);
    sqlite3_stmt *stmt;

    char *sql = "SELECT user_id, username from users u, friends f WHERE f.user1_id = ? AND f.user2_id = u.user_id";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

   cJSON *response_json = cJSON_CreateArray();
    if(stmt != NULL){
            sqlite3_bind_int(stmt, 1, user_id);
            while (sqlite3_step(stmt) != SQLITE_DONE){
                cJSON *user_json = cJSON_CreateObject();
                cJSON_AddNumberToObject(user_json, "user_id", sqlite3_column_int(stmt, 0));
                cJSON_AddStringToObject(user_json, "username", sqlite3_column_text(stmt, 1));
                cJSON_AddItemToArray(response_json, user_json);
            }
    }
    sqlite3_finalize(stmt);

    sqlite3_close(db);
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

    sqlite3 *db;
    sqlite3_open("chat-db.db", &db);
    sqlite3_stmt *stmt;

    char *sql = "INSERT INTO messages(user_id, friend_id, message, timestamp) VALUES(?, ?, ?, ?)";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if(stmt != NULL) {
        sqlite3_bind_int(stmt, 1, user_id);
        sqlite3_bind_int(stmt, 2, friend_id);
        sqlite3_bind_text(stmt, 3, message_str, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, timestamp_str, -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    sqlite3_stmt *stmt2;
    char *sql2 = "SELECT message_id FROM messages WHERE user_id = ? AND friend_id = ? AND message LIKE ? AND timestamp LIKE ?";
    sqlite3_prepare_v2(db, sql2, -1, &stmt2, 0);
    int message_id = 0;

    if(stmt2 != NULL) {
        sqlite3_bind_int(stmt2, 1, user_id);
        sqlite3_bind_int(stmt2, 2, friend_id);
        sqlite3_bind_text(stmt2, 3, message_str, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt2, 4, timestamp_str, -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        message_id = sqlite3_column_int(stmt2, 0);
        sqlite3_finalize(stmt);
    }


    cJSON *message_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(message_json, "id", message_id);
    cJSON_AddNumberToObject(message_json, "from", user_id);
    cJSON_AddNumberToObject(message_json, "to", friend_id);
    cJSON_AddStringToObject(message_json, "message", message_str);
    cJSON_AddStringToObject(message_json, "timestamp", timestamp_str);

    sqlite3_close(db);
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

    sqlite3 *db;
    sqlite3_open("chat-db.db", &db);
    sqlite3_stmt *stmt;

    char *sql = "SELECT * FROM messages WHERE (friend_id = ? AND user_id = ?) OR (friend_id = ? AND user_id = ?)";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    cJSON *response_json = cJSON_CreateArray();
    if(stmt != NULL){
        sqlite3_bind_int(stmt, 1, friend_id -> valueint);
        sqlite3_bind_int(stmt, 2, user_id);
        sqlite3_bind_int(stmt, 3, user_id);
        sqlite3_bind_int(stmt, 4, friend_id -> valueint);
        while (sqlite3_step(stmt) != SQLITE_DONE){
            cJSON *message_json = cJSON_CreateObject();
            cJSON_AddNumberToObject(message_json, "id", sqlite3_column_int(stmt, 0));
            cJSON_AddNumberToObject(message_json, "from", sqlite3_column_int(stmt, 1));
            cJSON_AddNumberToObject(message_json, "to", sqlite3_column_int(stmt, 2));
            cJSON_AddStringToObject(message_json, "message", sqlite3_column_text(stmt, 3));
            cJSON_AddStringToObject(message_json, "timestamp", sqlite3_column_text(stmt, 4));
            cJSON_AddItemToArray(response_json, message_json);
        }
    }
    sqlite3_finalize(stmt);

    sqlite3_close(db);
    *response_status = HTTP_OK;
    return response_json;
}
