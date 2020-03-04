/*
 * json_parse.c
 *
 *  Created on: Feb 19, 2020
 *      Author: Holden Ramsey
 */

#include "json_parse.h"

void json_receive(char *payload, MSG_TYPES msgType)
{
    int msgID = 0, item1 = 0, item2 = 0, item3 = 0;
    const char *rover_keys[4] = {"msgID", "item1", "item2", "item3"};
    const char *sensors_keys[4] = {"msgID", "item1", "item2", "item3"};
    const char *arm_keys[4] = {"msgID", "item1", "item2", "item3"};
    const char *zumo_keys[4] = {"msgID", "state", "leftmotor", "rightmotor"};
    switch(msgType)
    {
        case JSON_Rover:
        {
            json_read(payload, JSON_SENSORS, zumo_keys, &msgID, &item1, &item2, &item3);
        }
        case JSON_Sensors:
        {
            json_read(payload, JSON_SENSORS, sensors_keys, &msgID, &item1, &item2, &item3);
        }
        case JSON_Arm:
        {
            json_read(payload, JSON_SENSORS, rover_keys, &msgID, &item1, &item2, &item3);
        }
        case JSON_Zumo:
        {
            json_read(payload, JSON_ROVER, arm_keys, &msgID, &item1, &item2, &item3);
            //send data from here
        }
        default:
        {
            //ERROR;
        }
    }
}

void json_read(char *payload, char *msgType, const char *keys[4], int *msgID, int *item1, int *item2, int *item3)
{
    Json_Handle templateHandle, objectHandle;
    int ret1 = 0, ret2 = 0, ret3 = 0, ret4 = 0, ret5 = 0, ret6 = 0, ret7 = 0, ret8 = 0, ret9 = 0;
    uint16_t bufSize;
    ret1 = Json_createTemplate(&templateHandle, msgType, strlen(msgType));
    ret2 = Json_createObject(&objectHandle, templateHandle, 0);
    ret3 = Json_parse(objectHandle, payload, strlen(payload));
    bufSize = sizeof(msgID);
    ret4 = Json_getValue(objectHandle, keys[0], msgID, &bufSize);
    bufSize = sizeof(item1);
    ret5 = Json_getValue(objectHandle, keys[1], &item1, &bufSize);
    bufSize = sizeof(item2);
    ret6 = Json_getValue(objectHandle, keys[2], &item2, &bufSize);
    bufSize = sizeof(item3);
    ret7 = Json_getValue(objectHandle, keys[3], &item3, &bufSize);
    ret8 = Json_destroyObject(objectHandle);
    ret9 = Json_destroyTemplate(templateHandle);
    if(ret1 != 0  | ret2 != 0 | ret3 != 0 | ret4 != 0 | ret5 != 0 | ret6 != 0 | ret7 != 0 | ret8 != 0 | ret9 != 0)
    {
        //ERROR;
    }
}

void json_send(char *payload, MSG_TYPES msgType, int item1, int item2, int item3)
{
    static int statsID = 0, roverID = 0, sensorsID = 0, armID = 0, zumoID = 0;
    const char *stats_keys[4] = {"msgID", "item1", "item2", "item3"};
    const char *rover_keys[4] = {"msgID", "item1", "item2", "item3"};
    const char *sensors_keys[4] = {"msgID", "item1", "item2", "item3"};
    const char *arm_keys[4] = {"msgID", "item1", "item2", "item3"};
    const char *zumo_keys[4] = {"msgID", "state", "leftmotor", "rightmotor"};
    switch(msgType)
    {
        case JSON_Stats:
        {
            json_write(payload, JSON_STATS, stats_keys, statsID, item1, item2, item3);
            statsID++;
        }
        case JSON_Rover:
        {
            json_write(payload, JSON_ROVER, rover_keys, roverID, item1, item2, item3);
            roverID++;
        }
        case JSON_Sensors:
        {
            json_write(payload, JSON_SENSORS, sensors_keys, sensorsID, item1, item2, item3);
            sensorsID++;
        }
        case JSON_Arm:
        {
            json_write(payload, JSON_ARM, arm_keys, armID, item1, item2, item3);
            armID++;
        }
        case JSON_Zumo:
        {
            json_write(payload, JSON_ZUMO, zumo_keys, zumoID, item1, item2, item3);
            zumoID++;
        }
    }
}

void json_write(char *payload, char *msgType, const char *keys[4], int msgID, int item1, int item2, int item3)
{
    Json_Handle templateHandle;
    int ret1 = 0, ret2 = 0, ret3 = 0, ret4 = 0, ret5 = 0, ret6 = 0;
    uint16_t buf = PUBLISH_JSON_BUFFER_SIZE;
    ret1 = Json_createTemplate(&templateHandle, JSON_STATS, strlen(JSON_STATS));
    ret2 = Json_setValue(templateHandle, keys[0], &msgID, sizeof(msgID));
    ret3 = Json_setValue(templateHandle, keys[1], &item1, sizeof(item1));
    ret4 = Json_setValue(templateHandle, keys[2], &item2, sizeof(item2));
    ret5 = Json_setValue(templateHandle, keys[3], &item3, sizeof(item3));
    ret6 = Json_build(templateHandle, payload, &buf);
    //ret7 = Json_destroyObject(objectHandle);
    //ret8 = Json_destroyTemplate(templateHandle);
    if(ret1 != 0  | ret2 != 0 | ret3 != 0 | ret4 != 0 | ret5 != 0 | ret6 != 0)
    {
        //ERROR;
    }
}
