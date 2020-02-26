/*
 * UARTDebug_queue.c
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#include <UARTDebug_queue.h>
static QueueHandle_t xQueue = NULL;

void *UARTDebugThread(void *arg0)
{
    dbgOutputLoc(ENTER_TASK);
    uint16_t value, type;
    int received = 0;
    dbgOutputLoc(WHILE1);
    while(1)
    {
        received = receiveFromUARTDebugQ(&type, &value);
        if(received == -1)
        {
            ERROR;
        }
        else
        {
            if(type == LEFTCAP)
            {
                dbgUARTStr("Left Motor:");
                dbgUARTNum(value);
            }
            else if(type == RIGHTCAP)
            {
                dbgUARTStr("Right Motor:");
                dbgUARTNum(value);
            }
            else if(type == TIMER)
            {
                dbgUARTStr("Timer");
            }
        }
    }
}

void createUARTDebugQueue()
{
    xQueue = xQueueCreate(16, sizeof(uint32_t));
    if(xQueue == NULL)
    {
        ERROR;
    }
}

int sendMsgToUARTDebugQ(uint16_t type, uint16_t value)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    uint32_t msg = (type << UARTSHIFT) | value;
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
        *value = received;
    }
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
    return ret;
}
