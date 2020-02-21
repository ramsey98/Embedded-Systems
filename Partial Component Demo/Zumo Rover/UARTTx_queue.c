/*
 * UARTTx_queue.c
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#include "UARTTx_queue.h"
static QueueHandle_t xQueue = NULL;

void createUARTTxQueue()
{
    xQueue = xQueueCreate(16, sizeof(uint8_t));
    if(xQueue == NULL)
    {
        ERROR;
    }
}

int sendMsgToUARTTxQ(uint8_t value)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    uint8_t msg = value;
    success = xQueueSend(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER1);
    return ret;
}

int receiveFromUARTTxQ(uint8_t * value)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    uint8_t received;
    success = xQueueReceive(xQueue, &received, portMAX_DELAY);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    else
    {
        *value = received;
    }
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
    return ret;
}
