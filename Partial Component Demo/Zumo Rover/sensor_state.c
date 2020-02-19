/*
 * sensor_state.c
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#include "sensor_state.h"

int fsm(SENSOR_DATA *curState, int sensorVal)
{
    int success = 0;
    switch (curState->state)
    {
        case Init:
        {
            curState->sensorTotal = 0;
            curState->sensorCount = 0;
            curState->sensorAvg = 0;
            curState->state = Polling;
            break;
        }
        case Polling:
        {
            curState->sensorTotal += sensorVal;
            curState->sensorCount++;
            if (curState->sensorCount == 4)
            {
                curState->state = Averaging;
            }
            break;
        }
        case Averaging:
        {
            curState->sensorAvg = curState->sensorTotal/curState->sensorCount;
            curState->sensorTotal = 0;
            curState->sensorCount = 0;
            curState->state = Polling;
            break;
        }
        default:
        {
            success = -1;
            break;
        }
    }
    return success;
}


