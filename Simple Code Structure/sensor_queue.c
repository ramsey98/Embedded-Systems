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
    dbgOutputLoc(BEFORE_SEND_QUEUE);
    if(xQueueSendFromISR(xQueue, &timeVal, 0))
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
    dbgOutputLoc(BEFORE_SEND_QUEUE);
    if(xQueueSendFromISR(xQueue, &mmDist, 0))
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
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    if(xQueueReceive(xQueue, &itemToReceive, portMAX_DELAY))
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
