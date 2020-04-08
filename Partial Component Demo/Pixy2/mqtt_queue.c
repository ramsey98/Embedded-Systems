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
    if(xQueue == NULL) ERROR;
}

void sendMsgToMQTTQ(MQTTMsg msg)
{
    dbgOutputLoc(BEFORE_SEND_QUEUE_MQTT);
    BaseType_t success = xQueueSend(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE) ERROR;
    dbgOutputLoc(AFTER_SEND_QUEUE_MQTT);
}

void sendMsgToMQTTQFromISR(MQTTMsg msg)
{
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER2);
    BaseType_t success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE) ERROR;
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER2);
}

void receiveFromMQTTQ(MQTTMsg *received)
{
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    MQTTMsg temp;
    BaseType_t success = xQueueReceive(xQueue, &temp, portMAX_DELAY);
    if(success == pdFALSE) ERROR;
    *received = temp;
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
}


