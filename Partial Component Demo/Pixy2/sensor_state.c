/*
 * sensor_state.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Team 20
 */

#include "sensor_state.h"

int fsm(SENSOR_DATA *curState, int timeInc, int sensorVal)
{
    int success = 0;
    switch (curState->state)
    {
        case Init:
        {
            curState->curTime = 0;
            curState->sensorTotal = 0;
            curState->sensorCount = 0;
            curState->sensorAvg = 0;
            curState->state = WaitingForTime1;
            break;
        }
        case WaitingForTime1:
        {
            if (timeInc == 0)
            {
                curState->sensorTotal += sensorVal;
                curState->sensorCount++;
            }
            else if (timeInc > 0)
            {
                curState->curTime += timeInc;
                curState->sensorAvg = curState->sensorTotal/curState->sensorCount;
                dbgUARTStr("Sensor=:");
                dbgUARTNum(curState->sensorAvg);
                dbgUARTNum(curState->sensorCount);
                curState->sensorTotal = 0;
                curState->sensorCount = 0;
                curState->state = WaitingForTime2;
            }
            break;
        }
        case WaitingForTime2:
        {
            if (timeInc == 0)
            {
                curState->sensorTotal += sensorVal;
                curState->sensorCount++;
            }
            else if (timeInc > 0)
            {
                curState->curTime += timeInc;
                curState->sensorAvg = curState->sensorTotal/curState->sensorCount;
                dbgUARTStr("Sensor=:");
                dbgUARTNum(curState->sensorAvg);
                dbgUARTNum(curState->sensorCount);
                curState->sensorTotal = 0;
                curState->sensorCount = 0;
                curState->state = WaitingForTime3;
            }
            break;
        }
        case WaitingForTime3:
        {
            if (timeInc == 0)
            {
                curState->sensorTotal += sensorVal;
                curState->sensorCount++;
            }
            else if (timeInc > 0)
            {
                curState->curTime += timeInc;
                curState->sensorAvg = curState->sensorTotal/curState->sensorCount;
                dbgUARTStr("Sensor=:");
                dbgUARTNum(curState->sensorAvg);
                dbgUARTNum(curState->sensorCount);
                dbgUARTStr("CurTime=:");
                dbgUARTNum(curState->curTime/1000);
                curState->sensorTotal = 0;
                curState->sensorCount = 0;
                curState->state = WaitingForTime1;
            }
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


