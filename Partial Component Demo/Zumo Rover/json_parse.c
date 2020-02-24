/*
 * json_parse.c
 *
 *  Created on: Feb 19, 2020
 *      Author: Holden Ramsey
 */

#include "json_parse.h"


int json_read(char *payload, uint8_t *msgType, uint8_t *state)
{
    Json_Handle templateHandle, objectHandle;

    int ret = 0, ret1 = 0, ret2 = 0, ret3 = 0, ret4 = 0, ret5 = 0, ret6 = 0, ret7 = 0;
    uint16_t bufSize;
    ret1 = Json_createTemplate(&templateHandle, JSON_RECEIVE, strlen(JSON_RECEIVE));
    ret2 = Json_createObject(&objectHandle, templateHandle, 0);
    ret3 = Json_parse(objectHandle, payload, strlen(payload));
    char* key;
    key = "\"ID\"";
    bufSize = sizeof(msgType);
    ret4 = Json_getValue(objectHandle, key, msgType, &bufSize);
    if(*msgType == MQTT_STATE)
    {
        key = "\"state\"";
        bufSize = sizeof(state);
        ret5 = Json_getValue(objectHandle, key, state, &bufSize);
    }
    else
    {
        ERROR;
    }
    ret6 = Json_destroyObject(objectHandle);
    ret7 = Json_destroyTemplate(templateHandle);
    if(ret1 != 0  | ret2 != 0 | ret3 != 0 | ret4 != 0 | ret5 != 0 | ret6 != 0 | ret7 != 0)
    {
        ret = -1;
    }
    return ret;
}

int json_write(char *payload, uint8_t msgType, uint8_t state, uint8_t leftmotor, uint8_t rightmotor)
{
    Json_Handle templateHandle;
    int ret = 0, ret1 = 0, ret2 = 0, ret3 = 0, ret4 = 0;
    uint16_t buf = PUBLISH_JSON_BUFFER_SIZE;
    ret1 = Json_createTemplate(&templateHandle, JSON_SEND, strlen(JSON_SEND));
    char* key;
    key = "\"ID\"";
    ret2 = Json_setValue(templateHandle, key, &msgType, sizeof(msgType));
    if(msgType == MQTT_STATE)
    {
        key = "\"state\"";
        ret3 = Json_setValue(templateHandle, key, &state, sizeof(state));
    }
    else if(msgType == MQTT_LEFTMOTOR)
    {
        key = "\"leftmotor\"";
        ret3 = Json_setValue(templateHandle, key, &leftmotor, sizeof(leftmotor));
    }
    else if(msgType == MQTT_RIGHTMOTOR)
    {
        key = "\"rightmotor\"";
        ret3 = Json_setValue(templateHandle, key, &rightmotor, sizeof(rightmotor));
    }
    else
    {
        ERROR;
    }
    ret4 = Json_build(templateHandle, payload, &buf);
    //ret5 = Json_destroyObject(objectHandle);
    //ret6 = Json_destroyTemplate(templateHandle);
    if(ret1 != 0  | ret2 != 0 | ret3 != 0 | ret4 != 0)
    {
        ret = -1;
    }
    return ret;
}

