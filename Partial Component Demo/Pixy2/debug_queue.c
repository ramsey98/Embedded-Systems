/*
 * UARTDebug_queue.c
 *
 *  Created on: Feb 18, 2020
 *      Author: Matthew Conway
 */

#include <debug_queue.h>
static QueueHandle_t xQueue = NULL;

void createUARTDebugQueue()
{
    xQueue = xQueueCreate(16, sizeof(uint64_t));
    if(xQueue == NULL) ERROR;
}

void sendMsgToUARTDebugQ(int msg)
{
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER2);
    BaseType_t success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE) ERROR;
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER2);
}

void receiveFromUARTDebugQ(int *received)
{
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    int temp;
    BaseType_t success = xQueueReceive(xQueue, &temp, portMAX_DELAY);
    if(success == pdFALSE) ERROR;
    *received = temp;
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
}
