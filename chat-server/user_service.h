#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include "cJSON.h"
#include "http_status.h"
#include <stdbool.h>

int get_user_id(char *token);
cJSON *register_user(cJSON *request, int user_id, http_status *response_status);
cJSON *login(cJSON *request, int user_id, http_status *response_status);
cJSON *get_users(cJSON *request, int user_id, http_status *response_status);
cJSON *get_users_by_username(cJSON *request, int user_id, http_status *response_status);
cJSON *add_friend(cJSON *request, int user_id, http_status *response_status);
cJSON *delete_friend(cJSON *request, int user_id, http_status *response_status);
cJSON *get_friends(cJSON *request, int user_id, http_status *response_status);
void add_message(int user_id, int friend_id, char *message);
cJSON *get_messages(cJSON *request, int user_id, http_status *response_status);

#endif