/*
 * sensor_queue.c
 *
 *  Created on: Jan 25, 2020
 *      Author: Jon Glaser
 */

#include "sensor_queue.h"
static QueueHandle_t xQueue = NULL;

void createSensorQueue()
{
    xQueue = xQueueCreate(16, sizeof(uint32_t));
    if(!xQueue)
    {
        dbgOutputLoc(DBG_HALT);
    }
}

int sendTimeMsgToQ1(unsigned int timeVal)
{
    dbgOutputLoc(BEFORE_SEND_QUEUE);
    if(xQueueSendFromISR(xQueue, &timeVal, 0))
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int sendSensorMsgToQ1(int mmDist)
{
    dbgOutputLoc(BEFORE_SEND_QUEUE);
    if(xQueueSendFromISR(xQueue, &mmDist, 0))
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int receiveFromQ1()
{
    if(xQueueReceive(xQueue, &itemToReceive, portMAX_DELAY))
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
