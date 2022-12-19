#include "controller.h"
#include "cJSON.h"
#include "endpoints.h"
#include <string.h>

typedef cJSON *(*operation)(cJSON *request);

bool is_operation(request *request, char *method, char *path);
operation get_operation(request *request);

char *handle_request(request *request)
{
    cJSON *request_json = NULL;
    if (request->has_data)
    {
        request_json = cJSON_Parse(request->data);
    }

    operation operation = get_operation(request);
    cJSON *response_json = operation(request_json);
    char *response = get_resposne(HTTP_OK, response_json);

    if (response_json != NULL)
    {
        cJSON_Delete(response_json);
    }

    if (request->has_data)
    {
        cJSON_Delete(request_json);
    }
    return response;
}

bool is_operation(request *request, char *method, char *path)
{
    if (strcmp(request->method, method) == 0 && strcmp(request->path, path) == 0)
    {
        return true;
    }
    return false;
}

operation get_operation(request *request)
{
    if (strcmp(request->method, "OPTIONS") == 0)
    {
        return &get_null;
    }
    else if (is_operation(request, "POST", "/hello"))
    {
        return &get_hello;
    }
    return &get_null;
}