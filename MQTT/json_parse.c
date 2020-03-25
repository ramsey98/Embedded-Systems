/*
 * json_parse.c
 *
 *  Created on: Feb 19, 2020
 *      Author: Holden Ramsey
 */

#include "json_parse.h"

static int ID = 0, attempts = 0, received = 0, missed = 0, expected = 0;

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
    uint16_t buf = JSON_DATA_BUFFER_SIZE;
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
    if(Json_setValue(objectHandle, "\"ID\"", &ID, sizeof(ID)) != 0)
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
}

void json_send_debug(char *payload, MQTTMsg msg)
{
    Json_Handle templateHandle, objectHandle;
    uint16_t buf = JSON_DATA_BUFFER_SIZE;
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
    sendValues(objectHandle, msg, ID);
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
}

void json_send(char *publish_topic, char *publish_data, MQTTMsg msg)
{
    attempts++;
    memset(publish_data, 0, JSON_DATA_BUFFER_SIZE);
    if(msg.type == 1)
    {
        json_send_stats(publish_data);
        strncpy(publish_topic, PUBLISH_TOPIC_0, sizeof(PUBLISH_TOPIC_0));
    }
    else if(msg.type == 2)
    {
        json_send_debug(publish_data, msg);
        strncpy(publish_topic, PUBLISH_TOPIC_1, sizeof(PUBLISH_TOPIC_1));
    }
    else
    {
        ERROR;
    }
    if(sizeof(publish_data) > JSON_DATA_BUFFER_SIZE)
    {
       ERROR;
    }
    if(sizeof(publish_topic) > JSON_TOPIC_BUFFER_SIZE)
    {
       ERROR;
    }
    ID++;
}
