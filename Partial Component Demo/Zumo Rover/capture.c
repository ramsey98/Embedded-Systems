/*
 * capture.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#include "capture.h"

volatile uint32_t curInterval;
static Capture_Handle capture0, capture1;

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

    Capture_start(capture0);
    Capture_start(capture1);
}

void captureCallback(Capture_Handle handle, uint32_t interval)
{
    static int leftCount = 0, rightCount = 0;
    curInterval = interval;
    int sent;
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
    if(sent == -1)
    {
        ERROR;
    }
}
