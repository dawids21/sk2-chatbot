#ifndef ENDPOINTS_H
#define ENDPOINTS_H

#include "cJSON.h"
#include "requests.h"

cJSON *get_null(cJSON *request, http_status *response_status);
cJSON *get_hello(cJSON *request, http_status *response_status);
cJSON *post_register(cJSON *request, http_status *response_status);

#endif