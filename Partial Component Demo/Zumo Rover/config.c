/*
 * config.c
 *
 *  Created on: Apr 5, 2020
 *      Author: Holden Ramsey
 */

#include <navigation.h>
#include "config.h"

static QueueHandle_t xQueue = NULL;

void * configThread(void *arg0)
{
    int msg;
    while(1)
    {
        receiveFromConfigQ(&msg);
        if(msg == ROVER_LOADING)
        {
            sendMsgToNaviQ(PAUSE, EMPTY);
        }
        else if(msg == ROVER_MOVING)
        {
            sendMsgToNaviQ(RESUME, EMPTY);
        }
    }
}

void createConfigQueue()
{
    xQueue = xQueueCreate(16, sizeof(int));
    if(xQueue == NULL) ERROR;
}

void sendMsgToConfigQ(int msg)
{
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER);
    BaseType_t success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE) ERROR;
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER);
}

void receiveFromConfigQ(int *received)
{
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    int temp;
    BaseType_t success = xQueueReceive(xQueue, &temp, portMAX_DELAY);
    if(success == pdFALSE) ERROR;
    *received = temp;
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
}


