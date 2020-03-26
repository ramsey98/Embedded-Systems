/*
 * definitions.c
 *
 *  Created on: Mar 11, 2020
 *      Author: Holden Ramsey
 */

#include "definitions.h"
#include "uart_term.h"
#include <stdio.h>

static int value = 0;

void parseValues(Json_Handle objectHandle)
{
    uint16_t bufSize;
    int msgID = 0, item1 = 0;
    bufSize = sizeof(msgID);
    if(Json_getValue(objectHandle, "\"ID\"", &msgID, &bufSize) != 0)
    {
        ERROR;
    }
    bufSize = sizeof(item1);
    if(Json_getValue(objectHandle, "\"item1\"", &item1, &bufSize) != 0)
    {
        ERROR;
    }
    //send data where needed (queue)
    value = item1;
}

void sendValues(Json_Handle objectHandle, MQTTMsg msg, int msgID)
{
    int item1 = 0;
    if(msg.type == 2)
    {
        item1 = value;
    }
    else if(msg.type == 3)
    {
        item1 = msg.value;
    }
    if(Json_setValue(objectHandle, "\"ID\"", &msgID, sizeof(msgID)) != 0)
    {
        ERROR;
    }
    if(Json_setValue(objectHandle, "\"item1\"", &item1, sizeof(item1)) != 0)
    {
        ERROR;
    }
}
