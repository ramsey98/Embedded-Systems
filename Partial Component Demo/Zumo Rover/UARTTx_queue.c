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
    xQueue = xQueueCreate(16, sizeof(uint16_t));
    if(xQueue == NULL)
    {
        ERROR;
    }
}

void sendMsgToUARTTxQ(uint8_t byte1, uint8_t byte2)
{
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER);
    uint16_t msg = (byte1 << 8) | byte2;
    BaseType_t success = xQueueSend(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ERROR;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER);
}

void receiveFromUARTTxQ(uint16_t * value)
{
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    uint16_t received;
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
