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
        //receiveFromDebugQ(&value); todo fix later, recieveFromUARTDebugQ
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

void createDebugQueue()
{
    xQueue = xQueueCreate(16, sizeof(int));
    if(xQueue == NULL) ERROR;
}

void sendMsgToDebugQ(int msg)
{
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER2);
    BaseType_t success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE) ERROR;
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER2);
}

void receiveFromDebugQ(int *received)
{
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    int temp;
    BaseType_t success = xQueueReceive(xQueue, &temp, portMAX_DELAY);
    if(success == pdFALSE) ERROR;
    *received = temp;
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
}



