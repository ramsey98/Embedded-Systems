/*
 * capture.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#include "capture.h"

volatile uint32_t curInterval;
static Capture_Handle capture0, capture1;
static int leftCount, rightCount;

void clearCounts()
{
    leftCount = 0;
    rightCount = 0;
}

int getLeftCount()
{
    return leftCount;
}

int getRightCount()
{
    return rightCount;
}

void captureInit()
{
    Capture_Params captureParams;
    Capture_Params_init(&captureParams);
    captureParams.mode = Capture_RISING_EDGE; //detect two rising edges
    captureParams.periodUnit = Capture_PERIOD_US; //microseconds
    captureParams.callbackFxn = captureCallback;

    capture0 = Capture_open(CONFIG_CAPTURE_0, &captureParams);
    capture1 = Capture_open(CONFIG_CAPTURE_1, &captureParams);
    if (capture0 == NULL | capture1 == NULL)
    {
        ERROR;
    }

    leftCount = 0;
    rightCount = 0;
    if (Capture_start(capture0) == Capture_STATUS_ERROR | Capture_start(capture1) == Capture_STATUS_ERROR)
    {
        ERROR;
    }
}

void captureCallback(Capture_Handle handle, uint32_t interval)
{
    int sent;
    curInterval = interval;
    if(handle == capture0)
    {
        sent = sendMsgToPIDQ(LEFTCAP, curInterval);
        leftCount++;
    }
    else if(handle == capture1)
    {
        sent = sendMsgToPIDQ(RIGHTCAP, curInterval);
        rightCount++;
    }
    else
    {
        ERROR;
    }
    if(sent == -1)
    {
        ERROR;
    }
}
