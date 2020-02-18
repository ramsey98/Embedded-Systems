/*
 * capture.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#include "capture.h"

volatile uint32_t curInterval0;
volatile uint32_t curInterval1;
static SemaphoreP_Handle captureSem0;
static SemaphoreP_Handle captureSem1;

void capture0Init()
{
    SemaphoreP_Params semParams;
    Capture_Params captureParams;
    Capture_Handle capture0;
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    captureSem0 = SemaphoreP_create(0, &semParams);

    if (captureSem0 == NULL)
    {
        halt(); //could not allocate semaphore
    }

    Capture_Params_init(&captureParams);
    captureParams.mode = Capture_RISING_EDGE; //detect two rising edges
    captureParams.periodUnit = Capture_PERIOD_US; //microseconds
    captureParams.callbackFxn = capture0Callback;

    capture0 = Capture_open(CONFIG_CAPTURE_0, &captureParams);
    if (capture0 == NULL)
    {
        halt(); //failed to initialize capture
    }

    Capture_start(capture0);

    while(1)
    {
        SemaphoreP_pend(captureSem0, SemaphoreP_WAIT_FOREVER); //curInterval is period
    }

}

void capture1Init()
{
    SemaphoreP_Params semParams;
    Capture_Params captureParams;
    Capture_Handle capture1;
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    captureSem1 = SemaphoreP_create(0, &semParams);

    if (captureSem0 == NULL || captureSem1 == NULL)
    {
        halt(); //could not allocate semaphore
    }

    Capture_Params_init(&captureParams);
    captureParams.mode = Capture_RISING_EDGE; //detect two rising edges
    captureParams.periodUnit = Capture_PERIOD_US; //microseconds
    captureParams.callbackFxn = capture1Callback;

    capture1 = Capture_open(CONFIG_CAPTURE_1, &captureParams);
    if (capture1 == NULL)
    {
        halt(); //failed to initialize capture
    }

    Capture_start(capture1);

    while(1)
    {
        SemaphoreP_pend(captureSem1, SemaphoreP_WAIT_FOREVER);
    }

}

void capture0Callback(Capture_Handle handle, uint32_t interval)
{
    curInterval0 = interval;
    SemaphoreP_post(captureSem0);
    sendLeftMsgToCapQ(curInterval0);
}

void capture1Callback(Capture_Handle handle, uint32_t interval)
{
    curInterval1 = interval;
    SemaphoreP_post(captureSem1);
    sendRightMsgToCapQ(curInterval1);
}
