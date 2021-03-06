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
            case LEFTCAP:
                dbgUARTStr("Left Motor:");
                dbgUARTNum(value);
                break;
            case RIGHTCAP:
                dbgUARTStr("Right Motor:");
                dbgUARTNum(value);
                break;
            case TIMER:
                dbgUARTStr("Timer");
                break;
            case NAVI_SENSOR:
                dbgUARTStr("NAVI Sensor:");
                if(value == ACCEL)
                {
                    dbgUARTStr("ACCEL");
                }
                else if(value == DECEL)
                {
                    dbgUARTStr("DECEL");
                }
                break;
            case SENSOR:
                dbgUARTStr("Sensor:");
                dbgUARTNum(value);
                break;
            case PIXY:
                dbgUARTStr("Scaled:");
                dbgUARTNum(value);
                break;
            case PIXY_X:
                dbgUARTStr("Pixy X:");
                dbgUARTNum(value);
                break;
            case PIXY_Y:
                dbgUARTStr("Pixy Y:");
                dbgUARTNum(value);
                break;
            case PID_BEFORE:
                dbgUARTStr("PID Before:");
                dbgUARTNum(value);
                break;
            case PID_AFTER:
                dbgUARTStr("PID After:");
                dbgUARTNum(value);
                break;
            case CONFIG_MSG:
                dbgUARTStr("Config:");
                dbgUARTNum(value);
                break;
            default:
                dbgUARTStr("Invalid Debug");
                break;
        }
        dbgUARTStr(lineBreak);
    }
}

void createUARTDebugQueue()
{
    xQueue = xQueueCreate(16, sizeof(uint64_t));
    if(xQueue == NULL) ERROR;
}

void sendMsgToUARTDebugQ(uint32_t type, uint32_t value)
{
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER);
    uint64_t msg1 = type;
    uint64_t msg = (msg1 << UARTSHIFT) | value;
    BaseType_t success = xQueueSend(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE) ERROR;
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER);
}

void sendMsgToUARTDebugQFromISR(uint32_t type, uint32_t value)
{
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER);
    uint64_t msg1 = type;
    uint64_t msg = (msg1 << UARTSHIFT) | value;
    BaseType_t success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE) ERROR;
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER);
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
