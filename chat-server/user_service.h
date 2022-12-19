#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include "cJSON.h"
#include "http_status.h"
#include <stdbool.h>

int get_user_id(char *token);
cJSON *register_user(cJSON *request, int user_id, http_status *response_status);
cJSON *login(cJSON *request, int user_id, http_status *response_status);
cJSON *get_users(cJSON *request, int user_id, http_status *response_status);

#endif