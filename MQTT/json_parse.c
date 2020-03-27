/*
 * json_parse.c
 *
 *  Created on: Feb 19, 2020
 *      Author: Holden Ramsey
 */

#include "json_parse.h"

static int attempts = 0, received = 0, missed = 0;
static int debugValue = 0;

void json_miss()
{
    missed++;
}

void json_receive(char *payload, char *msgTopic)
{
    Json_Handle templateHandle, objectHandle;
    static int configID = 0, expectedConfigID = 0;
    uint16_t bufSize;
    received++;
    if(strcmp(msgTopic, SUBSCRIPTION_TOPIC) == 0)
    {
        if(Json_createTemplate(&templateHandle, JSON_CONFIG, strlen(JSON_CONFIG)) != 0) ERROR;
        if(Json_createObject(&objectHandle, templateHandle, 0) != 0) ERROR;
        if(Json_parse(objectHandle, payload, strlen(payload)) != 0) ERROR;
        bufSize = sizeof(configID);
        if(Json_getValue(objectHandle, "\"ID\"", &configID, &bufSize) != 0) ERROR;
        if(configID > expectedConfigID)
        {
            missed += (configID - expectedConfigID);
            MQTTMsg msg = {JSON_TYPE_ERROR, JSON_ERROR_MISSED_ID};
            sendMsgToMQTTQ(msg);
            expectedConfigID = configID + 1;
            json_read_config(objectHandle);
        }
        else if(configID == expectedConfigID - 1)
        {
            MQTTMsg msg = {JSON_TYPE_ERROR, JSON_ERROR_REPEAT_ID};
            sendMsgToMQTTQ(msg);
        }
        else if(configID == expectedConfigID)
        {
            expectedConfigID = configID + 1;
            json_read_config(objectHandle);
        }
        if(Json_destroyTemplate(templateHandle) != 0) ERROR;
        if(Json_destroyObject(objectHandle) != 0) ERROR;
    }
}

void json_read_config(Json_Handle objectHandle)
{
    uint16_t bufSize;
    int item1 = 0;
    bufSize = sizeof(item1);
    if(Json_getValue(objectHandle, "\"item1\"", &item1, &bufSize) != 0)
    {
        MQTTMsg msg = {JSON_TYPE_ERROR, JSON_ERROR_FORMAT};
        sendMsgToMQTTQ(msg);
    }
    else
    {
        debugValue = item1;
    }
}

void json_send_stats(Json_Handle objectHandle)
{
    static int statsID = 0;
    if(Json_parse(objectHandle, JSON_STATS_BUF, strlen(JSON_STATS_BUF)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"ID\"", &statsID, sizeof(statsID)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"Attempts\"", &attempts, sizeof(attempts)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"Received\"", &received, sizeof(received)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"Missed\"", &missed, sizeof(missed)) != 0) ERROR;
    statsID++;
}

void json_send_debug(MQTTMsg msg, Json_Handle objectHandle)
{
    static int debugID = 0;
    int item1 = debugValue;
    if(Json_parse(objectHandle, JSON_DEBUG_BUF, strlen(JSON_DEBUG_BUF)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"ID\"", &debugID, sizeof(debugID)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"item1\"", &item1, sizeof(item1)) != 0) ERROR;
    debugID++;
}

void json_send_error(MQTTMsg msg, Json_Handle objectHandle)
{
    static int errorsID = 0;
    int error = msg.value;
    if(Json_parse(objectHandle, JSON_ERRORS_BUF, strlen(JSON_ERRORS_BUF)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"ID\"", &errorsID, sizeof(errorsID)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"Type\"", &error, sizeof(error)) != 0) ERROR;
    errorsID++;
}

void json_send(char *publish_topic, char *publish_data, MQTTMsg msg)
{
    Json_Handle templateHandle, objectHandle;
    uint16_t buf = JSON_DATA_BUFFER_SIZE;
    attempts++;
    memset(publish_data, 0, JSON_DATA_BUFFER_SIZE);
    if(msg.type == JSON_TYPE_STATS)
    {
        if(Json_createTemplate(&templateHandle, JSON_STATS, strlen(JSON_STATS)) != 0) ERROR;
        if(Json_createObject(&objectHandle, templateHandle, 0) != 0) ERROR;
        json_send_stats(objectHandle);
        strncpy(publish_topic, PUBLISH_TOPIC_0, sizeof(PUBLISH_TOPIC_0));
    }
    else if(msg.type == JSON_TYPE_DEBUG)
    {
        if(Json_createTemplate(&templateHandle, JSON_DEBUG, strlen(JSON_DEBUG)) != 0) ERROR;
        if(Json_createObject(&objectHandle, templateHandle, 0) != 0) ERROR;
        json_send_debug(msg, objectHandle);
        strncpy(publish_topic, PUBLISH_TOPIC_1, sizeof(PUBLISH_TOPIC_1));
    }
    else if(msg.type == JSON_TYPE_ERROR)
    {
        if(Json_createTemplate(&templateHandle, JSON_ERRORS, strlen(JSON_ERRORS)) != 0) ERROR;
        if(Json_createObject(&objectHandle, templateHandle, 0) != 0) ERROR;
        json_send_error(msg, objectHandle);
        strncpy(publish_topic, PUBLISH_TOPIC_2, sizeof(PUBLISH_TOPIC_2));
    }
    else
    {
        ERROR;
    }
    if(Json_build(objectHandle, publish_data, &buf) != 0) ERROR;
    if(Json_destroyTemplate(templateHandle) != 0) ERROR;
    if(Json_destroyObject(objectHandle) != 0) ERROR;
    if(sizeof(publish_data) > JSON_DATA_BUFFER_SIZE) ERROR;
    if(sizeof(publish_topic) > JSON_TOPIC_BUFFER_SIZE) ERROR;
}


