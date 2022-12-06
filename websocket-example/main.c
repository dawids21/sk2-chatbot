#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "wsServer/ws.h"
#include "cJSON.h"

ws_cli_conn_t *clients[100];

void onopen(ws_cli_conn_t *client)
{
    char *cli;
    cli = ws_getaddress(client);
    printf("Connection opened, addr: %s\n", cli);
}

void onclose(ws_cli_conn_t *client)
{
    char *cli;
    cli = ws_getaddress(client);
    printf("Connection closed, addr: %s\n", cli);
}

void onmessage(ws_cli_conn_t *client,
               const unsigned char *msg, uint64_t size, int type)
{
    const char *message = (const char *)msg;
    cJSON *json = cJSON_Parse(message);
    cJSON *operation = cJSON_GetObjectItemCaseSensitive(json, "operation");
    if (strcmp(operation->valuestring, "/start") == 0)
    {
        cJSON *user_id = cJSON_GetObjectItemCaseSensitive(json, "user_id");
        cJSON *token = cJSON_GetObjectItemCaseSensitive(json, "token");
        // validate token...
        clients[user_id->valueint] = client;
    }
    else if (strcmp(operation->valuestring, "/message") == 0)
    {
        cJSON *to = cJSON_GetObjectItemCaseSensitive(json, "to");
        ws_cli_conn_t *receipent = clients[to->valueint];
        ws_sendframe_txt(receipent, message);
    }
    cJSON_Delete(json);
}

int main(void)
{
    struct ws_events evs;
    evs.onopen = &onopen;
    evs.onclose = &onclose;
    evs.onmessage = &onmessage;

    ws_socket(&evs, 5000, 0, 1000);

    return (0);
}