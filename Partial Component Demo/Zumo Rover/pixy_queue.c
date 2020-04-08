/*
 * pixy_queue.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#include "pixy_queue.h"
static QueueHandle_t xQueue = NULL;

void createPixyQueue()
{
    xQueue = xQueueCreate(16, sizeof(uint8_t));
    if(xQueue == NULL) ERROR;
}

void sendMsgToPixyQ(uint8_t type)
{
    dbgOutputLoc(BEFORE_SEND_QUEUE);
    uint8_t msg = type;
    BaseType_t success = xQueueSend(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE) ERROR;
    dbgOutputLoc(AFTER_SEND_QUEUE);
}

void sendMsgToPixyQFromISR(uint8_t type)
{
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER);
    uint8_t msg = type;
    BaseType_t success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE) ERROR;
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER);
}

void receiveFromPixyQ(uint8_t *type)
{
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    uint8_t received;
    BaseType_t success = xQueueReceive(xQueue, &received, portMAX_DELAY);
    if(success == pdFALSE) ERROR;
    *type = received;
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
}


