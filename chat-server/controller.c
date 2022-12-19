#include "controller.h"
#include "cJSON.h"
#include <string.h>
#include "user_service.h"
#include "http_status.h"

typedef cJSON *(*operation)(cJSON *request, int user_id, http_status *response_status);

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
    http_status status;
    char *token = NULL;
    if (request->has_auth)
    {
        token = request->auth;
    }
    int user_id = get_user_id(token);
    cJSON *response_json = operation(request_json, user_id, &status);
    char *response = get_response(status, response_json);

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

cJSON *get_method_not_allowed(cJSON *request, int user_id, http_status *response_status);

operation get_operation(request *request)
{
    if (is_operation(request, "POST", "/register"))
    {
        return &register_user;
    }
    else if (is_operation(request, "POST", "/login"))
    {
        return &login;
    }
    else if (is_operation(request, "GET", "/users"))
    {
        return &get_users;
    }
    else if (is_operation(request, "GET", "/users/username"))
    {
        return &get_users_by_username;
    }
    else if (is_operation(request, "POST", "/friends"))
    {
        return &add_friend;
    }
    else if (is_operation(request, "DELETE", "/friends"))
    {
        return &delete_friend;
    }
    else if (is_operation(request, "GET", "/friends"))
    {
        return &get_friends;
    }
    else if (is_operation(request, "GET", "/messages"))
    {
        return &get_messages;
    }
    return &get_method_not_allowed;
}

cJSON *get_method_not_allowed(cJSON *request, int user_id, http_status *response_status)
{
    *response_status = HTTP_METHOD_NOT_ALLOWED;
    return NULL;
}