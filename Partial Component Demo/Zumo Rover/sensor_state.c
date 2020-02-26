/*
 * sensor_state.c
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#include "sensor_state.h"

void sensor_fsm(SENSOR_DATA *curState, int sensorVal)
{
    switch (curState->state)
    {
        case Sensor_Init:
        {
            curState->sensorTotal = 0;
            curState->sensorCount = 0;
            curState->sensorAvg = 0;
            curState->state = Sensor_Polling;
            break;
        }
        case Sensor_Polling:
        {
            curState->sensorTotal += sensorVal;
            curState->sensorCount++;
            if (curState->sensorCount == 5)
            {
                curState->state = Sensor_Averaging;
            }
            break;
        }
        case Sensor_Averaging:
        {
            curState->sensorAvg = curState->sensorTotal/curState->sensorCount;
            //sendMsgToPIDQ(SENSOR, curState->sensorAvg);
            if(curState->sensorAvg > 65)
            {
                sendMsgToPIDQ(ACCEL, 25);
            }
            else if(curState->sensorAvg < 55)
            {
                sendMsgToPIDQ(DECEL, 25);
            }
            curState->sensorTotal = 0;
            curState->sensorCount = 0;
            curState->state = Sensor_Polling;
            break;
        }
        default:
        {
            ERROR;
            break;
        }
    }
}


