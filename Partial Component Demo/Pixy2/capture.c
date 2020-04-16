/*
 * capture.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Matthew Conway
 */

#include "capture.h"

volatile uint32_t curInterval;
static Capture_Handle capture0;

void captureInit()
{
    Capture_Params captureParams;
    Capture_Params_init(&captureParams);
    captureParams.mode = Capture_ANY_EDGE;
    captureParams.periodUnit = Capture_PERIOD_US;
    captureParams.callbackFxn = captureCallback;
    capture0 = Capture_open(CONFIG_CAPTURE_0, &captureParams);
    if (capture0 == NULL) ERROR;
    if (Capture_start(capture0) == Capture_STATUS_ERROR) ERROR;
}

void captureCallback(Capture_Handle handle, uint32_t interval)
{
    if(interval < 90000) { //should capture every other edge (ie falling)
        curInterval = interval;
        sendSensorMsgToQ(curInterval);
    }
}

