/*
 * sensor_state.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Jon Glaser
 */

#include "sensor_state.h"

int fsm(STATES curState, int timeInc, int sensorVal)
{
    int success = 1;
    switch (curState)
    {
        case Init:
        {
            GPIO_init();
            UART_init();
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
                uartOut = "Sensor=:\r\n";
                UART_write(uart, uartOut, sizeof(uartOut));
                UART_write(uart, sensorAvg, sizeof(sensorAvg));
                UART_write(uart, sensorCount, sizeof(sensorCount));
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
                uartOut = "Sensor=:\r\n";
                UART_write(uart, uartOut, sizeof(uartOut));
                UART_write(uart, sensorAvg, sizeof(sensorAvg));
                UART_write(uart, sensorCount, sizeof(sensorCount));
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
                uartOut = "Sensor=:\r\n";
                UART_write(uart, uartOut, sizeof(uartOut));
                uartOut = sensorAvg;
                UART_write(uart, uartOut, sizeof(uartOut));
                uartOut = sensorCount;
                UART_write(uart, uartOut, sizeof(uartOut));
                uartOut = "CurTime=:\r\n";
                UART_write(uart, uartOut, sizeof(uartOut));
                uartOut = curTime;
                UART_write(uart, uartOut, sizeof(uartOut));
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

void *mainThread(void *arg0)
{
    while(1)
    {
        //receive from queue
    }
}


