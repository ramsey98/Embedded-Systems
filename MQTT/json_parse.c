/*
 * json_parse.c
 *
 *  Created on: Feb 19, 2020
 *      Author: Holden Ramsey
 */

#include "json_parse.h"

static int statsID = 0, dataID = 0, attempts = 0, received = 0, missed = 0, expected = 0;

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
    if(Json_createTemplate(&templateHandle, JSON_SUB, strlen(JSON_SUB)) != 0)
    {
        ERROR;
    }
    if(Json_createObject(&objectHandle, templateHandle, 0) != 0)
    {
        ERROR;
    }
    if(Json_parse(objectHandle, JSON_SUB_BUF, strlen(JSON_SUB_BUF)) != 0)
    {
        ERROR;
    }
    parseValues(objectHandle);
    if(Json_destroyTemplate(templateHandle) != 0)
    {
        ERROR;
    }
    if(Json_destroyObject(objectHandle) != 0)
    {
        ERROR;
    }
}

void json_send_stats(char *payload)
{
    Json_Handle templateHandle, objectHandle;
    uint16_t buf = PUBLISH_JSON_BUFFER_SIZE;
    if(Json_createTemplate(&templateHandle, JSON_STATS, strlen(JSON_STATS)) != 0)
    {
        ERROR;
    }
    if(Json_createObject(&objectHandle, templateHandle, 0) != 0)
    {
        ERROR;
    }
    if(Json_parse(objectHandle, JSON_STATS_BUF, strlen(JSON_STATS_BUF)) != 0)
    {
        ERROR;
    }
    if(Json_setValue(objectHandle, "\"ID\"", &statsID, sizeof(statsID)) != 0)
    {
        ERROR;
    }
    if(Json_setValue(objectHandle, "\"Attempts\"", &attempts, sizeof(attempts)) != 0)
    {
        ERROR;
    }
    if(Json_setValue(objectHandle, "\"Received\"", &received, sizeof(received)) != 0)
    {
        ERROR;
    }
    if(Json_setValue(objectHandle, "\"Missed\"", &missed, sizeof(missed)) != 0)
    {
        ERROR;
    }
    if(Json_build(objectHandle, payload, &buf) != 0)
    {
        ERROR;
    }
    if(Json_destroyTemplate(templateHandle) != 0)
    {
        ERROR;
    }
    if(Json_destroyObject(objectHandle) != 0)
    {
        ERROR;
    }
    attempts++;
    statsID++;
}

void json_send_data(char *payload, MQTTMsg msg)
{
    Json_Handle templateHandle, objectHandle;
    uint16_t buf = PUBLISH_JSON_BUFFER_SIZE;
    if(Json_createTemplate(&templateHandle, JSON_PUB, strlen(JSON_PUB)) != 0)
    {
        ERROR;
    }
    if(Json_createObject(&objectHandle, templateHandle, 0) != 0)
    {
        ERROR;
    }
    if(Json_parse(objectHandle, JSON_PUB_BUF, strlen(JSON_PUB_BUF)) != 0)
    {
        ERROR;
    }
    sendValues(objectHandle, msg, dataID);
    if(Json_build(objectHandle, payload, &buf) != 0)
    {
        ERROR;
    }
    if(Json_destroyTemplate(templateHandle) != 0)
    {
        ERROR;
    }
    if(Json_destroyObject(objectHandle) != 0)
    {
        ERROR;
    }
    attempts++;
    dataID++;
}
