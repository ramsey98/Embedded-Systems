/*
 * motors_queue.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#include <navi_queue.h>

static QueueHandle_t xQueue = NULL;

void createNaviQueue()
{
    xQueue = xQueueCreate(16, sizeof(uint64_t));
    if(xQueue == NULL) ERROR;
}

void sendMsgToNaviQ(uint32_t type, uint32_t value)
{
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER);
    uint64_t msg1 = type;
    uint64_t msg = (msg1 << NAVISHIFT) | value;
    BaseType_t success = xQueueSend(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE) ERROR;
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER);
}

void sendMsgToNaviQFromISR(uint32_t type, uint32_t value)
{
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER);
    uint64_t msg1 = type;
    uint64_t msg = (msg1 << NAVISHIFT) | value;
    BaseType_t success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE) ERROR;
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER);
}

void receiveFromNaviQ(uint32_t * type, uint32_t * value)
{
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    uint64_t received;
    BaseType_t success = xQueueReceive(xQueue, &received, portMAX_DELAY);
    if(success == pdFALSE) ERROR;
    *type = received >> NAVISHIFT;
    *value = received;
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
}
