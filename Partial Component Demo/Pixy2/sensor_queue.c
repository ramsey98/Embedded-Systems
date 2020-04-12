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
    xQueue = xQueueCreate(16, sizeof(int));
    if(xQueue == NULL) ERROR;
}

int sendSensorMsgToQ(int value)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER2);
    int msg = value;
    success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE) ERROR;
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER2);
    return ret;
}

int receiveFromSensorQ()
{
    BaseType_t success;
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    int received;
    success = xQueueReceive(xQueue, &received, portMAX_DELAY);
    if(success == pdFALSE) ERROR;
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
    return received;
}
