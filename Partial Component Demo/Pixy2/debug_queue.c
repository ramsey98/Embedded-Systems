/*
 * UARTDebug_queue.c
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#include <debug_queue.h>
static QueueHandle_t xQueue = NULL;

void *UARTDebugThread(void *arg0)
{
    dbgOutputLoc(ENTER_TASK);
    uint32_t value, type;
    char lineBreak[] = "\n\r";
    dbgOutputLoc(WHILE1);
    while(1)
    {
        receiveFromUARTDebugQ(&type, &value);
        switch(type)
        {
            case PIXY_COLOR:
                dbgUARTStr("Color:");
                dbgUARTNum(value);
                dbgUARTStr(", ");
                break;
            case PIXY_DISTANCE:
                dbgUARTStr("Distance:");
                dbgUARTNumAsChars(value);
                dbgUARTStr(lineBreak);
                break;
            case SENSOR:
                dbgUARTStr("Sensor:");
                dbgUARTNumAsChars(value);
                dbgUARTStr(lineBreak);
                break;
            default:
                dbgUARTStr("Invalid Debug");
                dbgUARTStr(lineBreak);
                break;
        }
    }
}

void createUARTDebugQueue()
{
    xQueue = xQueueCreate(16, sizeof(uint64_t));
    if(xQueue == NULL) ERROR;
}

void sendMsgToUARTDebugQ(uint32_t type, uint32_t value)
{
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER2);
    uint64_t msg1 = type;
    uint64_t msg = (msg1 << UARTSHIFT) | value;
    BaseType_t success = xQueueSend(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE) ERROR;
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER2);
}

void sendMsgToUARTDebugQFromISR(uint32_t type, uint32_t value)
{
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER2);
    uint64_t msg1 = type;
    uint64_t msg = (msg1 << UARTSHIFT) | value;
    BaseType_t success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE) ERROR;
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER2);
}

void receiveFromUARTDebugQ(uint32_t * type, uint32_t * value)
{
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    uint64_t received;
    BaseType_t success = xQueueReceive(xQueue, &received, portMAX_DELAY);
    if(success == pdFALSE) ERROR;
    *type = received >> UARTSHIFT;
    *value = received;
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
}
