/*
 * json_parse.c
 *
 *  Created on: Feb 19, 2020
 *      Author: Holden Ramsey
 */

#include "json_parse.h"

static int attempts = 0, received = 0, missed = 0;

void json_miss(int count, int type)
{
    MQTTMsg msg = {JSON_TYPE_ERROR, type};
    sendMsgToMQTTQFromISR(msg);
    missed+=count;
}

void json_receive(char *payload, char *msgTopic)
{
    Json_Handle templateHandle, objectHandle;
    static int expectedConfigID = 0;
    int configID = 0;
    uint16_t bufSize;
    received++;
    if(strcmp(msgTopic, SUBSCRIPTION_TOPIC) == 0)
    {
        if(Json_createTemplate(&templateHandle, JSON_CONFIG, strlen(JSON_CONFIG)) != 0) ERROR;
        if(Json_createObject(&objectHandle, templateHandle, 0) != 0) ERROR;
        if(Json_parse(objectHandle, payload, strlen(payload)) != 0) ERROR;
        bufSize = sizeof(configID);
        if(Json_getValue(objectHandle, "\"ID\"", &configID, &bufSize) != 0)
        {
            json_miss(1, JSON_ERROR_NO_ID);
        }
        else
        {
            if(configID > expectedConfigID)
            {
                json_miss(configID - expectedConfigID, JSON_ERROR_MISSED_ID);
                expectedConfigID = configID + 1;
                json_read_config(objectHandle);
            }
            else if(configID == expectedConfigID - 1)
            {
                json_miss(1, JSON_ERROR_REPEAT_ID);
            }
            else if(configID == expectedConfigID)
            {
                expectedConfigID = configID + 1;
                json_read_config(objectHandle);
            }
        }
        if(Json_destroyTemplate(templateHandle) != 0) ERROR;
        if(Json_destroyObject(objectHandle) != 0) ERROR;
    }
}

void json_read_config(Json_Handle objectHandle)
{
    uint16_t bufSize;
    int value = 0;
    bufSize = sizeof(value);
    if(Json_getValue(objectHandle, "\"value\"", &value, &bufSize) != 0)
    {
        json_miss(1, JSON_ERROR_FORMAT);
    }
    else
    {
        sendMsgToConfigQ(value);
    }
}

void json_send_stats(Json_Handle objectHandle)
{
    dbgOutputLoc(ENTER_SEND_STATS);
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
    dbgOutputLoc(ENTER_SEND_DEBUG);
    static int debugID = 0;
    int value = msg.value1;
    if(Json_parse(objectHandle, JSON_DEBUG_BUF, strlen(JSON_DEBUG_BUF)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"ID\"", &debugID, sizeof(debugID)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"value\"", &value, sizeof(value)) != 0) ERROR;
    debugID++;
}

void json_send_error(MQTTMsg msg, Json_Handle objectHandle)
{
    dbgOutputLoc(ENTER_SEND_ERROR);
    static int errorsID = 0;
    int error = msg.value1;
    if(Json_parse(objectHandle, JSON_ERRORS_BUF, strlen(JSON_ERRORS_BUF)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"ID\"", &errorsID, sizeof(errorsID)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"Type\"", &error, sizeof(error)) != 0) ERROR;
    errorsID++;
}

void json_send_pixy(MQTTMsg msg, Json_Handle objectHandle) {
    dbgOutputLoc(ENTER_SEND_PIXY);
    static int pixyID = 0;
    int color = msg.value1;
    int dist = msg.value2;
    int offset = msg.value4;
    if(Json_parse(objectHandle, JSON_PIXY_BUF, strlen(JSON_PIXY_BUF)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"ID\"", &pixyID, sizeof(pixyID)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"Color\"", &color, sizeof(color)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"Distance\"", &dist, sizeof(dist)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"Offset\"", &offset, sizeof(offset)) != 0) ERROR;
    pixyID++;
}

void json_send_ultrasonic(MQTTMsg msg, Json_Handle objectHandle) {
    dbgOutputLoc(ENTER_SEND_ULTRASONIC);
    static int ultrasonicID = 0;
    int dist = msg.value4;
    if(Json_parse(objectHandle, JSON_ULTRASONIC_BUF, strlen(JSON_ULTRASONIC_BUF)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"ID\"", &ultrasonicID, sizeof(ultrasonicID)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"Distance\"", &dist, sizeof(dist)) != 0) ERROR;
    ultrasonicID++;
}


void json_send_polls(MQTTMsg msg, Json_Handle objectHandle) {
    dbgOutputLoc(ENTER_SEND_POLLS);
    static int pollID = 0;
    int pixy = msg.value1;
    int sensor = msg.value2;
    if(Json_parse(objectHandle, JSON_POLLS_BUF, strlen(JSON_POLLS_BUF)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"ID\"", &pollID, sizeof(pollID)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"PixyPolls\"", &pixy, sizeof(pixy)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"SensorPolls\"", &sensor, sizeof(sensor)) != 0) ERROR;
    pollID++;
}

void json_send_sensorstate(MQTTMsg msg, Json_Handle objectHandle) {
    dbgOutputLoc(ENTER_SEND_SENSORSTATE);
    static int sensorstateID = 0;
    int type = msg.value1;
    int pos = msg.value2;
    int distance = msg.value3;
    int offset = msg.value4;
    if(Json_parse(objectHandle, JSON_SENSORSTATE_BUF, strlen(JSON_SENSORSTATE_BUF)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"ID\"", &sensorstateID, sizeof(sensorstateID)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"Type\"", &type, sizeof(type)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"Position\"", &pos, sizeof(pos)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"Distance\"", &distance, sizeof(distance)) != 0) ERROR;
    if(Json_setValue(objectHandle, "\"Offset\"", &offset, sizeof(offset)) != 0) ERROR;
    sensorstateID++;
}


void json_send(char *publish_topic, char *publish_data, MQTTMsg msg)
{
    Json_Handle templateHandle, objectHandle;
    uint16_t buf = JSON_DATA_BUFFER_SIZE;
    attempts++;
    memset(publish_data, 0, JSON_DATA_BUFFER_SIZE);
    switch(msg.type)
    {
        case JSON_TYPE_STATS:
            if(Json_createTemplate(&templateHandle, JSON_STATS, strlen(JSON_STATS)) != 0) ERROR;
            if(Json_createObject(&objectHandle, templateHandle, 0) != 0) ERROR;
            json_send_stats(objectHandle);
            strncpy(publish_topic, PUBLISH_TOPIC_0, sizeof(PUBLISH_TOPIC_0));
            break;
        case JSON_TYPE_DEBUG:
            if(Json_createTemplate(&templateHandle, JSON_DEBUG, strlen(JSON_DEBUG)) != 0) ERROR;
            if(Json_createObject(&objectHandle, templateHandle, 0) != 0) ERROR;
            json_send_debug(msg, objectHandle);
            strncpy(publish_topic, PUBLISH_TOPIC_1, sizeof(PUBLISH_TOPIC_1));
            break;
        case JSON_TYPE_ERROR:
            if(Json_createTemplate(&templateHandle, JSON_ERRORS, strlen(JSON_ERRORS)) != 0) ERROR;
            if(Json_createObject(&objectHandle, templateHandle, 0) != 0) ERROR;
            json_send_error(msg, objectHandle);
            strncpy(publish_topic, PUBLISH_TOPIC_2, sizeof(PUBLISH_TOPIC_2));
            break;
        case JSON_TYPE_PIXY:
            if(Json_createTemplate(&templateHandle, JSON_PIXY, strlen(JSON_PIXY)) != 0) ERROR;
            if(Json_createObject(&objectHandle, templateHandle, 0) != 0) ERROR;
            json_send_pixy(msg, objectHandle);
            strncpy(publish_topic, PUBLISH_TOPIC_3, sizeof(PUBLISH_TOPIC_3));
            break;
        case JSON_TYPE_ULTRASONIC:
            if(Json_createTemplate(&templateHandle, JSON_ULTRASONIC, strlen(JSON_ULTRASONIC)) != 0) ERROR;
            if(Json_createObject(&objectHandle, templateHandle, 0) != 0) ERROR;
            json_send_ultrasonic(msg, objectHandle);
            strncpy(publish_topic, PUBLISH_TOPIC_4, sizeof(PUBLISH_TOPIC_4));
            break;
        case JSON_TYPE_POLLS:
            if(Json_createTemplate(&templateHandle, JSON_POLLS, strlen(JSON_POLLS)) != 0) ERROR;
            if(Json_createObject(&objectHandle, templateHandle, 0) != 0) ERROR;
            json_send_polls(msg, objectHandle);
            strncpy(publish_topic, PUBLISH_TOPIC_5, sizeof(PUBLISH_TOPIC_5));
            break;
        case JSON_TYPE_SENSORSTATE:
            if(Json_createTemplate(&templateHandle, JSON_SENSORSTATE, strlen(JSON_SENSORSTATE)) != 0) ERROR;
            if(Json_createObject(&objectHandle, templateHandle, 0) != 0) ERROR;
            json_send_sensorstate(msg, objectHandle);
            strncpy(publish_topic, PUBLISH_TOPIC_6, sizeof(PUBLISH_TOPIC_6));
            break;
        default:
            ERROR;
    }

    if(Json_build(objectHandle, publish_data, &buf) != 0) ERROR;
    if(Json_destroyTemplate(templateHandle) != 0) ERROR;
    if(Json_destroyObject(objectHandle) != 0) ERROR;
    if(sizeof(publish_data) > JSON_DATA_BUFFER_SIZE) ERROR;
    if(sizeof(publish_topic) > JSON_TOPIC_BUFFER_SIZE) ERROR;
}


