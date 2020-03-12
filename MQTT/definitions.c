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
    ret2 = Json_getValue(objectHandle, "state", &state, &bufSize);
    if(ret1 != 0  | ret2 != 0)
    {
        ERROR;
    }
    else
    {
        //send data where needed
    }
}


