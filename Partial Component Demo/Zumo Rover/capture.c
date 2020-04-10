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
    if (capture0 == NULL) ERROR;
    if (capture1 == NULL) ERROR;
    if (Capture_start(capture0) == Capture_STATUS_ERROR) ERROR;
    if (Capture_start(capture1) == Capture_STATUS_ERROR) ERROR;
}

void captureCallback(Capture_Handle handle, uint32_t interval)
{
    curInterval = interval;
    if(handle == capture0)
    {
        sendMsgToPIDQFromISR(LEFTCAP, curInterval);
    }
    else if(handle == capture1)
    {
        sendMsgToPIDQFromISR(RIGHTCAP, curInterval);
    }
    else ERROR;
}
