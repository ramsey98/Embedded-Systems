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

void *captureThread(void *arg0)
{
    capture0Init();
    capture1Init();
    dbgOutputLoc(ENTER_TASK);
    uint8_t prevleftFreq = 0;
    uint8_t prevrightFreq = 0;
    uint8_t leftFreq = 0;
    uint8_t rightFreq = 0;
    int received = 0;
    dbgOutputLoc(WHILE1);
    while(1)
    {
        received = receiveFromCapQ(&leftFreq, &rightFreq);
        if(received == -1)
        {
            ERROR;
        }
        if(leftFreq != prevleftFreq)
        {
            //sendLeftMotorMsgToMQTTQ(leftFreq);
            prevleftFreq = leftFreq;
        }
        if(rightFreq != prevrightFreq)
        {
            //sendRightMotorMsgToMQTTQ(rightFreq);
            prevrightFreq = rightFreq;
        }
    }
}

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
        ERROR; //could not allocate semaphore
    }

    Capture_Params_init(&captureParams);
    captureParams.mode = Capture_RISING_EDGE; //detect two rising edges
    captureParams.periodUnit = Capture_PERIOD_US; //microseconds
    captureParams.callbackFxn = capture0Callback;

    capture0 = Capture_open(CONFIG_CAPTURE_0, &captureParams);
    if (capture0 == NULL)
    {
        ERROR;
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

    if (captureSem1 == NULL)
    {
        ERROR;
    }

    Capture_Params_init(&captureParams);
    captureParams.mode = Capture_RISING_EDGE; //detect two rising edges
    captureParams.periodUnit = Capture_PERIOD_US; //microseconds
    captureParams.callbackFxn = capture1Callback;

    capture1 = Capture_open(CONFIG_CAPTURE_1, &captureParams);
    if (capture1 == NULL)
    {
        ERROR;
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
    uint8_t freq = SECOND/curInterval0;
    sendLeftMsgToCapQ(freq);
}

void capture1Callback(Capture_Handle handle, uint32_t interval)
{
    curInterval1 = interval;
    SemaphoreP_post(captureSem1);
    uint8_t freq = SECOND/curInterval1;
    sendRightMsgToCapQ(freq);
}
