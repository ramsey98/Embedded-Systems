/*
 * json_parse.c
 *
 *  Created on: Feb 19, 2020
 *      Author: Holden Ramsey
 */

#include "json_parse.h"

const char *stats_keys[4] = {"ID", "Attempts", "Received", "Missed"};
static int received = 0, missed = 0, expected = 0;

void json_miss()
{
    missed++;
}

void json_receive(char *payload, char *msgTopic)
{
    int msgID = 0;
    if(strcmp(msgTopic, SUBSCRIPTION_TOPIC) == 0)
    {
        received++;
        json_read(payload, &msgID);
        if(msgID > expected)
        {
            missed += (msgID - expected);
        }
        expected = msgID + 1;
    }
}

void json_read(char *payload, int *msgID)
{
    Json_Handle templateHandle, objectHandle;
    int ret1 = 0, ret2 = 0, ret3 = 0, ret4 = 0, ret5 = 0;

    ret1 = Json_createTemplate(&templateHandle, JSON_FORMAT, strlen(JSON_FORMAT));
    ret2 = Json_createObject(&objectHandle, templateHandle, 0);
    ret3 = Json_parse(objectHandle, payload, strlen(payload));
    parseValues(objectHandle);
    ret4 = Json_destroyObject(objectHandle);
    ret5 = Json_destroyTemplate(templateHandle);
    if(ret1 != 0  | ret2 != 0 | ret3 != 0 | ret4 != 0 | ret5 != 0)
    {
        ERROR;
    }
}

void json_send_stats(char *payload)
{
    static int statsID = 0, attempts = 0;
    Json_Handle templateHandle;
    int ret1 = 0, ret2 = 0, ret3 = 0, ret4 = 0, ret5 = 0, ret6 = 0;
    uint16_t buf = PUBLISH_JSON_BUFFER_SIZE;
    ret1 = Json_createTemplate(&templateHandle, JSON_STATS, strlen(JSON_STATS));
    ret2 = Json_setValue(templateHandle, "ID", &statsID, sizeof(statsID));
    ret3 = Json_setValue(templateHandle, "Attempts", &attempts, sizeof(attempts));
    ret4 = Json_setValue(templateHandle, "Received", &received, sizeof(received));
    ret5 = Json_setValue(templateHandle, "Missed", &missed, sizeof(missed));
    ret6 = Json_build(templateHandle, payload, &buf);
    if(ret1 != 0  | ret2 != 0 | ret3 != 0 | ret4 != 0 | ret5 != 0 | ret6 != 0)
    {
        ERROR;
    }
    attempts++;
    statsID++;
}
