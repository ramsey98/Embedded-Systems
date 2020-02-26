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
    xQueue = xQueueCreate(16, sizeof(uint16_t));
    if(xQueue == NULL)
    {
        ERROR;
    }
}

void sendSensorMsgToQ(uint16_t mmDist)
{
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER2);
    uint16_t msg = mmDist;
    BaseType_t success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ERROR;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER2);
}

void receiveFromSensorQ(uint16_t * sensorVal)
{
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    uint32_t received;
    BaseType_t success = xQueueReceive(xQueue, &received, portMAX_DELAY);
    if(success == pdFALSE)
    {
        ERROR;
    }
    else
    {
        *sensorVal = received;
    }
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
}
