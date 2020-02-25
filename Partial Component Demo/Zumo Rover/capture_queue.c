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
        ERROR;
    }
}

int sendLeftMsgToCapQ(uint32_t period)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    uint64_t msg = 0x0000000100000000 | period;//(LEFT << ENCODERSHIFT) | period;
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
    uint64_t msg = 0x0000000200000000 | period;//(RIGHT << ENCODERSHIFT) | period;
    success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER1);
    return ret;
}

int receiveFromCapQ(uint32_t * type, uint32_t * period)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    uint64_t received;
    success = xQueueReceive(xQueue, &received, portMAX_DELAY);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    else
    {
        if (received >> ENCODERSHIFT == LEFT)
        {
            *type = LEFT;
        }
        else if (received >> ENCODERSHIFT == RIGHT)
        {
            *type = RIGHT;
        }
        *period = (received & CAPFMASK);
    }
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
    return ret;
}
