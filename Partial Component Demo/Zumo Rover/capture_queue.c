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
    xQueue = xQueueCreate(16, sizeof(uint16_t));
    if(xQueue == NULL)
    {
        ERROR;
    }
}

int sendLeftMsgToCapQ(uint8_t freq)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    uint16_t msg = LEFTFLAG | freq;
    success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER1);
    return ret;
}

int sendRightMsgToCapQ(uint8_t freq)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    uint16_t msg = RIGHTFLAG | freq;
    success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER1);
    return ret;
}

int receiveFromCapQ(uint8_t * leftFreq, uint8_t * rightFreq)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    uint16_t received;
    success = xQueueReceive(xQueue, &received, portMAX_DELAY);

    if (received >> ENCODERSHIFT == LEFTMASK)
    {
        *leftFreq = (received & CAPFMASK);
    }
    else if (received >> ENCODERSHIFT == RIGHTMASK)
    {
        *rightFreq = (received & CAPFMASK);
    }
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
    return ret;
}
