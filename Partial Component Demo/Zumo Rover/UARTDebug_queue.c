/*
 * UARTDebug_queue.c
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#include <UARTDebug_queue.h>
static QueueHandle_t xQueue = NULL;

void createUARTDebugQueue()
{
    xQueue = xQueueCreate(16, sizeof(uint32_t));
    if(xQueue == NULL)
    {
        ERROR;
    }
}

int sendLeftCapMsgToUARTDebugQ(uint16_t value)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    uint32_t msg = (1 << 16) | value;
    success = xQueueSend(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER1);
    return ret;
}

int sendRightCapMsgToUARTDebugQ(uint16_t value)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    uint32_t msg = (2 << 16) | value;
    success = xQueueSend(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER1);
    return ret;
}

int receiveFromUARTDebugQ(uint16_t * type, uint16_t * value)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    uint32_t received;
    success = xQueueReceive(xQueue, &received, portMAX_DELAY);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    else
    {
        *type = received >> 16;
        *value = received & 0xFFFF;
    }
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
    return ret;
}
