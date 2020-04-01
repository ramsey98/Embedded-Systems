/*
 * mqtt_queue.c
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#include "debug_queue.h"
static QueueHandle_t xQueue = NULL;

void createDebugQueue()
{
    xQueue = xQueueCreate(16, sizeof(int));
    if(xQueue == NULL) ERROR;
}

void sendMsgToDebugQ(int msg)
{
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER);
    BaseType_t success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE) ERROR;
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER);
}

void receiveFromDebugQ(int *received)
{
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    int temp;
    BaseType_t success = xQueueReceive(xQueue, &temp, portMAX_DELAY);
    if(success == pdFALSE) ERROR;
    *received = temp;
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
}


