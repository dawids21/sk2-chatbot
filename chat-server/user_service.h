#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include "cJSON.h"
#include "http_status.h"

cJSON *register_user(cJSON *request, http_status *response_status);

#endif