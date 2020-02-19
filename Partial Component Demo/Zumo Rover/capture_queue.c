/*
 * capture_queue.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#include "capture_queue.h"
static QueueHandle_t xQueue = NULL;

void createCaptureQueue()
{
    xQueue = xQueueCreate(16, sizeof(uint64_t));
    if(xQueue == NULL)
    {
        halt();
    }
}

int sendLeftMsgToCapQ(uint32_t period)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    uint64_t msg = LEFTFLAG | period;
    success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER1);
    return ret;
}

int sendRightMsgToCapQ(uint32_t period)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    uint64_t msg = RIGHTFLAG | period;
    success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER1);
    return ret;
}

int receiveFromCapQ(uint32_t * leftFreq, uint32_t * rightFreq)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    uint64_t received;
    success = xQueueReceive(xQueue, &received, portMAX_DELAY);

    if (received >> ENCODERSHIFT == LEFTMASK)
    {
        *leftFreq = 1000000 / (received & CAPFMASK);
    }
    else if (received >> ENCODERSHIFT == RIGHTMASK)
    {
        *rightFreq = 1000000 / (received & CAPFMASK);
    }
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
    return ret;
}
