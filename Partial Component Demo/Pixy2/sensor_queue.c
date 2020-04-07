/*
 * sensor_queue.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Team 20
 */

#include "sensor_queue.h"
static QueueHandle_t xQueueSensor = NULL;

void createSensorQueue()
{
    xQueueSensor = xQueueCreate(16, sizeof(uint64_t));
    if(xQueueSensor == NULL)
    {
        ERROR;
    }
}

int sendTimeMsgToQ1(unsigned int timeVal)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    uint64_t msg = TIMEFLAG | timeVal;
    success = xQueueSendFromISR(xQueueSensor, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER1);
    return ret;
}

int sendSensorMsgToQ1(int mmDist)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER2);

    uint64_t msg = SENSORFLAG | mmDist;
    success = xQueueSendFromISR(xQueueSensor, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER2);
    return ret;
}

int receiveFromQ1(int * timeInc, int * sensorVal)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    uint64_t received;
    success = xQueueReceive(xQueueSensor, &received, portMAX_DELAY);

    if (received >> SHIFT == TIMEMASK)
    {
        *timeInc = received & FMASK;
    }
    else if (received >> SHIFT == SENSORMASK)
    {
        *sensorVal = received & FMASK;
        *timeInc = 0;
    }
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
    return ret;
}
