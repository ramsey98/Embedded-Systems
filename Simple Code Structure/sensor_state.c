/*
 * sensor_state.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Jon Glaser
 */

#include "sensor_state.h"
#include <stdio.h>


int fsm(STATES curState, int timeInc, int sensorVal)
{
    int success = 1;
    switch (curState)
    {
        case Init:
        {
            GPIO_init();
            UART_init();
            UART_Params_init(&uartParams);
            uartParams.writeDataMode = UART_DATA_BINARY;
            uartParams.readDataMode = UART_DATA_BINARY;
            uartParams.readReturnMode = UART_RETURN_FULL;
            uartParams.readEcho = UART_ECHO_OFF;
            uartParams.baudRate = 115200;
            uart = UART_open(CONFIG_UART_0, &uartParams);
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
                sprintf(uartOut, "%d", sensorAvg);
                UART_write(uart, uartOut, sizeof(uartOut));
                sprintf(uartOut, "%d", sensorCount);
                UART_write(uart, uartOut, sizeof(uartOut));
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
                sprintf(uartOut, "%d", sensorAvg);
                UART_write(uart, uartOut, sizeof(uartOut));
                sprintf(uartOut, "%d", sensorCount);
                UART_write(uart, uartOut, sizeof(uartOut));
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
                sprintf(uartOut, "%d", sensorAvg);
                UART_write(uart, uartOut, sizeof(uartOut));
                sprintf(uartOut, "%d", sensorCount);
                UART_write(uart, uartOut, sizeof(uartOut));
                uartOut = "CurTime=:\r\n";
                UART_write(uart, uartOut, sizeof(uartOut));
                sprintf(uartOut, "%d", curTime);
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
    Timer_Handle timer0;
    Timer_Params params;
    Timer_init();
    Timer_Params_init(&params);
    params.period = 75000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timer75Callback;

    timer0 = Timer_open(CONFIG_TIMER_0, &params);
    if (timer0 == NULL)
    {
        halt();
    }

    if (Timer_start(timer0) == Timer_STATUS_ERROR)
    {
        halt();
    }

    return (NULL);
}

void timer75Callback(Timer_Handle myHandle)
{
    //read from queue
    //fsm(curState, timeInc, sensorVal);
}

