/*
 * mqtt_queue.c
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#include "MQTT_queue.h"
static QueueHandle_t xQueue = NULL;

void createMQTTQueue()
{
    xQueue = xQueueCreate(16, sizeof(MQTTMsg));
    if(xQueue == NULL)
    {
        ERROR;
    }
}

void sendMsgToMQTTQ(MQTTMsg msg)
{
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER);
    BaseType_t success = xQueueSend(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ERROR;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER);
}

void receiveFromMQTTQ(MQTTMsg *received)
{
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    BaseType_t success = xQueueReceive(xQueue, &received, portMAX_DELAY);
    if(success == pdFALSE)
    {
        ERROR;
    }
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
}


