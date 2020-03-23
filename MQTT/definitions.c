/*
 * definitions.c
 *
 *  Created on: Mar 11, 2020
 *      Author: Holden Ramsey
 */

#include "definitions.h"

void parseValues(Json_Handle objectHandle)
{
    uint16_t bufSize;
    int msgID = 0;
    char state = 0;
    bufSize = sizeof(msgID);
    if(Json_getValue(objectHandle, "\"ID\"", &msgID, &bufSize) != 0)
    {
        ERROR;
    }
    bufSize = sizeof(state);
    if(Json_getValue(objectHandle, "\"item1\"", &state, &bufSize) != 0)
    {
        ERROR;
    }
    //send data where needed
    dbgUARTNum(state);
}

void sendValues(Json_Handle objectHandle, MQTTMsg msg, int msgID)
{
    uint8_t item1, item2;
    item1 = msg.type;
    item2 = msg.value;
    if(Json_setValue(objectHandle, "\"ID\"", &msgID, sizeof(msgID)) != 0)
    {
        ERROR;
    }
    if(Json_setValue(objectHandle, "\"item1\"", &item1, sizeof(item1)) != 0)
    {
        ERROR;
    }
    if(Json_setValue(objectHandle, "\"item2\"", &item2, sizeof(item2)) != 0)
    {
        ERROR;
    }
}
