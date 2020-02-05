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
    if(!xQueue)
    {
        dbgOutputLoc(DBG_HALT);
    }
}

int sendTimeMsgToQ1(unsigned int timeVal)
{
    int ret;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE);
    success = xQueueSendFromISR(xQueue, &timeVal, pdFALSE);
    if(success == pdTRUE)
    {
        ret = 0;
    }
    else
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE);
    return ret;
}

int sendSensorMsgToQ1(int mmDist)
{
    int ret;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE);
    success = xQueueSendFromISR(xQueue, &mmDist, pdFALSE);
    if(success == pdTRUE)
    {
        ret = 0;
    }
    else
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE);
    return ret;
}

int receiveFromQ1( void * itemToReceive )
{
    int ret;
    BaseType_t success;
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    success = xQueueReceive(xQueue, &itemToReceive, portMAX_DELAY);
    if(success == pdTRUE)
    {
        ret = 0;
    }
    else
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
    return ret;
}
