/*
 * sensor_thread.c
 *
 *  Created on: Mar 3, 2020
 *      Author: Matthew
 */

#include "sensor_thread.h"

void *sensorThread(void *arg0) {

    dbgOutputLoc(ENTER_TASK);
    int received = 0, averageTime = 0;
    static int sumSensorTime = 0;
    static int numberOfPolls = 0;
    static int distance = 0;
    SemaphoreP_Params semParams;
    Capture_Params captureParams;
    Capture_Handle capture;

    /* Semaphore to wait for capture data */
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    captureSem = SemaphoreP_create(0, &semParams);
    if(captureSem == NULL) ERROR;

    /* Setting up the Capture driver to detect two rising edges and report
     * the result in microseconds
     */
    Capture_Params_init(&captureParams);
    captureParams.mode = Capture_RISING_EDGE_FALLING_EDGE;
    captureParams.periodUnit = Capture_PERIOD_US;
    captureParams.callbackFxn = captureCallback;
    capture = Capture_open(CONFIG_CAPTURE_0, &captureParams);
    if (capture == NULL) ERROR;
    Capture_start(capture);

    timerTriggerInit();

    while(1) {
        received = receiveFromSensorQ();

        /*
        switch(received) {
            case SENSOR_TYPE_TRIGGER:
                //send 10 us trigger
                GPIO_write(CONFIG_GPIO_8, CONFIG_GPIO_LED_ON);
                timerTriggerStart();

                // The value printed should be close to the period of the pwm
                SemaphoreP_pend(captureSem, SemaphoreP_WAIT_FOREVER);
                sumSensorTime += curInterval;
                numberOfPolls++;
                break;
            case SENSOR_TYPE_SUM:
                averageTime = sumSensorTime/numberOfPolls;
                distance = (CM_PER_US * averageTime)/2;
                dbgUARTStr("Sensord: ");
                dbgUARTNumAsChars(distance);
                sumSensorTime = 0;
                numberOfPolls = 0;
                break;
        } */
    }
}

/* Callback function that displays the interval */
void captureCallback(Capture_Handle handle, uint32_t interval)
{
    curInterval = interval;
    SemaphoreP_post(captureSem);
}
