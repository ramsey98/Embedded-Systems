/*
 * sensor_queue.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Team 20
 */

#include "sensor_queue.h"

static QueueHandle_t xQueue = NULL;

void createSensorQueue()
{
    xQueue = xQueueCreate(16, sizeof(uint64_t));
    if(xQueue == NULL)
    {
        ERROR;
    }
}

int sendTimeMsgToQ1(unsigned int timeVal)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    uint64_t msg = 0x0000000100000000 | timeVal;
    success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
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

    uint64_t msg = 0x1000000000000000 | mmDist;
    success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
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
    success = xQueueReceive(xQueue, &received, portMAX_DELAY);

    if (received >> 32 == 0x00000001)
    {

        *timeInc = received & 0xffffffff;
        *sensorVal = 0;
    }
    else if (received >> 32 == 0x10000000)
    {
        *sensorVal = received & 0xffffffff;
        *timeInc = 0;
    }
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
    return ret;
}
