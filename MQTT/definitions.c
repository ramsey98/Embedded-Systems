/*
 * definitions.c
 *
 *  Created on: Mar 11, 2020
 *      Author: Holden Ramsey
 */

#include "definitions.h"

void parseValues(Json_Handle objectHandle)
{
    int ret1 = 0, ret2 = 0;
    uint16_t bufSize;
    int msgID = 0;
    char state = 0;
    bufSize = sizeof(msgID);
    ret1 = Json_getValue(objectHandle, "ID", &msgID, &bufSize);
    bufSize = sizeof(state);
    ret2 = Json_getValue(objectHandle, "item1", &state, &bufSize);
    if(ret1 != 0  | ret2 != 0)
    {
        ERROR;
    }
    else
    {
        //send data where needed
    }
}

void sendValues(Json_Handle templateHandle, MQTTMsg msg, int msgID)
{
    int ret1 = 0, ret2 = 0, ret3 = 0;
    uint8_t item1, item2;
    item1 = msg.type;
    item2 = msg.value;
    ret1 = Json_setValue(templateHandle, "ID", &msgID, sizeof(msgID));
    ret2 = Json_setValue(templateHandle, "item1", &item1, sizeof(item1));
    ret3 = Json_setValue(templateHandle, "item2", &item2, sizeof(item2));
    if(ret1 != 0  | ret2 != 0 | ret3 != 0)
    {
        ERROR;
    }
}
