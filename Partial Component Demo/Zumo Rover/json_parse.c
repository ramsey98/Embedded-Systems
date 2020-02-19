/*
 * json_parse.c
 *
 *  Created on: Feb 19, 2020
 *      Author: Holden Ramsey
 */

#include "json_parse.h"


int json_read(char *payload, uint8_t *msgType, uint8_t *state, uint16_t *leftmotor, uint16_t *rightmotor)
{
    Json_Handle templateHandle;
    int ret = 0, ret1 = 0, ret2 = 0, ret3 = 0;
    ret1 = Json_parse(templateHandle, payload, strlen(payload));
    char* key;
    key = "\"ID\"";
    ret2 = Json_getValue(templateHandle, key, msgType, sizeof(msgType));
    if(msgType == MQTT_STATE)
    {
        key = "\"state\"";
        ret3 = Json_getValue(templateHandle, key, state, sizeof(state));
    }
    else if(msgType == MQTT_LEFTMOTOR)
    {
        key = "\"leftmotor\"";
        ret3 = Json_getValue(templateHandle, key, leftmotor, sizeof(leftmotor));
    }
    else if(msgType == MQTT_RIGHTMOTOR)
    {
        key = "\"rightmotor\"";
        ret3 = Json_getValue(templateHandle, key, rightmotor, sizeof(rightmotor));
    }
    else
    {
        halt();
    }
    if(ret1 < 0  | ret2 < 0 | ret3 < 0)
    {
        ret = -1;
    }
    return ret;
}

int json_write(char *payload, uint8_t msgType, uint8_t state, uint16_t leftmotor, uint16_t rightmotor)
{
    Json_Handle templateHandle;
    int ret = 0, ret1 = 0, ret2 = 0, ret3 = 0, ret4 = 0;
    uint16_t buf = PUBLISH_JSON_BUFFER_SIZE;
    ret1 = Json_createTemplate(&templateHandle, JSON_TEMPLATE, strlen(JSON_TEMPLATE));
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
        halt();
    }
    ret4 = Json_build(templateHandle, payload, &buf);
    if(ret1 < 0  | ret2 < 0 | ret3 < 0 | ret4 < 0)
    {
        ret = -1;
    }
    return ret;
}

