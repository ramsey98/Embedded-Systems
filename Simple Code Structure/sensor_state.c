/*
 * sensor_state.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Jon Glaser
 */

#include "sensor_state.h"
#include "debug.h"
#include <stdio.h>


int fsm(STATES curState, int timeInc, int sensorVal)
{
    int success = 1;
    switch (curState)
    {
        case Init:
        {

            curTime = 0;
            sensorTotal = 0;
            sensorCount = 0;
            sensorAvg = 0;
            curState = WaitingForTime1;
        }
        case WaitingForTime1:
        {
            if (timeInc == 0)
            {
                sensorTotal += sensorVal;
                sensorCount++;
            }
            else if (timeInc > 0)
            {
                curTime += timeInc;
                sensorAvg = sensorTotal/sensorCount;
                dbgUARTStr("Sensor=:\r\n");
                dbgUARTVal(sensorAvg);
                dbgUARTVal(sensorCount);
                sensorTotal = 0;
                sensorCount = 0;
            }
            curState = WaitingForTime2;
        }
        case WaitingForTime2:
        {
            if (timeInc == 0)
            {
                sensorTotal += sensorVal;
                sensorCount++;
            }
            else if (timeInc > 0)
            {
                curTime += timeInc;
                sensorAvg = sensorTotal/sensorCount;
                dbgUARTStr("Sensor=:\r\n");
                dbgUARTVal(sensorAvg);
                dbgUARTVal(sensorCount);
                sensorTotal = 0;
                sensorCount = 0;
            }
            curState = WaitingForTime3;
        }
        case WaitingForTime3:
        {
            if (timeInc == 0)
            {
                sensorTotal += sensorVal;
                sensorCount++;
            }
            else if (timeInc > 0)
            {
                curTime += timeInc;
                sensorAvg = sensorTotal/sensorCount;
                dbgUARTStr("Sensor=:\r\n");
                dbgUARTVal(sensorAvg);
                dbgUARTVal(sensorCount);
                dbgUARTStr("CurTime=:\r\n");
                dbgUARTVal(curTime);
                sensorTotal = 0;
                sensorCount = 0;
                curState = WaitingForTime1;
            }
        }
        default:
        {
            success = 0;
        }
    }
    return success;
}


