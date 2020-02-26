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
    if(xQueue == NULL)
    {
        ERROR;
    }
}

void sendMsgToPixyQ(uint32_t value)
{
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    uint8_t msg = value;
    BaseType_t success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ERROR;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER1);
}

void receiveFromPixyQ(uint32_t * value)
{
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    uint8_t received;
    BaseType_t success = xQueueReceive(xQueue, &received, portMAX_DELAY);
    if(success == pdFALSE)
    {
        ERROR;
    }
    else
    {
        *value = received;
    }
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
}


