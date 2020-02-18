/*
 * speed_queue.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#include "speed_queue.h"

static QueueHandle_t xQueue = NULL;

void createSpeedQueue()
{
    xQueue = xQueueCreate(16, sizeof(uint16_t));
    if(xQueue == NULL)
    {
        halt();
    }
}

int sendLeftMsgToSpeedQ(uint8_t value)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    uint16_t msg = LEFTSPEEDFLAG | value;
    success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER1);
    return ret;
}

int sendRightMsgToSpeedQ(uint8_t value)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    uint16_t msg = RIGHTSPEEDFLAG | value;
    success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER1);
    return ret;
}

int sendAccelMsgToSpeedQ(uint8_t value)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    uint16_t msg = ACCELFLAG | value;
    success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER1);
    return ret;
}

int sendDecelMsgToSpeedQ(uint8_t value)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    uint16_t msg = DECELFLAG | value;
    success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER1);
    return ret;
}

int sendPauseMsgToSpeedQ()
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    uint16_t msg = PAUSEFLAG;
    success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER1);
    return ret;
}

int sendResumeMsgToSpeedQ()
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    uint16_t msg = RESUMEFLAG;
    success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER1);
    return ret;
}

int receiveFromSpeedQ(uint16_t * value)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    uint16_t received;
    success = xQueueReceive(xQueue, &received, portMAX_DELAY);

    if (received >> SPEEDSHIFT == LEFTSPEEDMASK)
    {
        *value = received & SPEEDFMASK;
    }
    else if (received >> SPEEDSHIFT == RIGHTSPEEDMASK)
    {
    }
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
    return ret;
}

