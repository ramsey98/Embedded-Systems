/*
 * motors_queue.c
 *
 *  Created on: Feb 17, 2020
 *      Author: Holden Ramsey
 */

#include <PID_queue.h>

static QueueHandle_t xQueue = NULL;

void createPIDQueue()
{
    xQueue = xQueueCreate(16, sizeof(uint64_t));
    if(xQueue == NULL)
    {
        ERROR;
    }
}

int sendMsgToPIDQ(uint32_t type, uint32_t value)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    uint64_t msg1 = type;
    uint64_t msg = (msg1 << PIDSHIFT) | value; //shift count is too large
    success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER1);
    return ret;
}

int receiveFromPIDQ(uint32_t * type, uint32_t * value)
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
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
    return ret;
}
