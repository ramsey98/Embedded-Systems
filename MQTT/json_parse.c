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
        if(msgID > expected)
        {
            missed += (msgID - expected);
            MQTTMsg msg = {JSON_TYPE_ERROR, JSON_ERROR_MISSED_ID};
            sendMsgToMQTTQ(msg);
            json_read(payload, &msgID);
            expected = msgID + 1;
        }
        else if(msgID == expected - 1)
        {
            MQTTMsg msg = {JSON_TYPE_ERROR, JSON_ERROR_REPEAT_ID};
            sendMsgToMQTTQ(msg);
        }
        else if(msgID == expected)
        {
            json_read(payload, &msgID);
            expected = msgID + 1;
        }
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
    if(Json_parse(objectHandle, payload, strlen(payload)) != 0)
    {
        ERROR;
    }
    if(parseValues(objectHandle) != 0)
    {
        MQTTMsg msg = {JSON_TYPE_ERROR, JSON_ERROR_FORMAT};
        sendMsgToMQTTQ(msg);
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
    if(Json_createTemplate(&templateHandle, JSON_DEBUG, strlen(JSON_DEBUG)) != 0)
    {
        ERROR;
    }
    if(Json_createObject(&objectHandle, templateHandle, 0) != 0)
    {
        ERROR;
    }
    if(Json_parse(objectHandle, JSON_DEBUG_BUF, strlen(JSON_DEBUG_BUF)) != 0)
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

void json_send_error(char *payload, MQTTMsg msg)
{
    Json_Handle templateHandle, objectHandle;
    uint16_t buf = JSON_DATA_BUFFER_SIZE;
    int error = msg.value;
    if(Json_createTemplate(&templateHandle, JSON_ERROR, strlen(JSON_ERROR)) != 0)
    {
        ERROR;
    }
    if(Json_createObject(&objectHandle, templateHandle, 0) != 0)
    {
        ERROR;
    }
    if(Json_parse(objectHandle, JSON_ERROR_BUF, strlen(JSON_ERROR_BUF)) != 0)
    {
        ERROR;
    }
    if(Json_setValue(objectHandle, "\"ID\"", &ID, sizeof(ID)) != 0)
    {
        ERROR;
    }
    if(Json_setValue(objectHandle, "\"Type\"", &error, sizeof(error)) != 0)
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

void json_send(char *publish_topic, char *publish_data, MQTTMsg msg)
{
    attempts++;
    memset(publish_data, 0, JSON_DATA_BUFFER_SIZE);
    if(msg.type == JSON_TYPE_STATS)
    {
        json_send_stats(publish_data);
        strncpy(publish_topic, PUBLISH_TOPIC_0, sizeof(PUBLISH_TOPIC_0));
    }
    else if(msg.type == JSON_TYPE_DEBUG)
    {
        json_send_debug(publish_data, msg);
        strncpy(publish_topic, PUBLISH_TOPIC_1, sizeof(PUBLISH_TOPIC_1));
    }
    else if(msg.type == JSON_TYPE_ERROR)
    {
        json_send_error(publish_data, msg);
        strncpy(publish_topic, PUBLISH_TOPIC_2, sizeof(PUBLISH_TOPIC_2));
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


