/*
 * distance_queue.c
 *
 *  Created on: Mar 4, 2020
 *      Author: Matthew
 */

#include "distance_queue.h"
static QueueHandle_t xQueueDistance = NULL;

void createDistanceQueue()
{
    xQueueDistance = xQueueCreate(16, sizeof(DISTANCE_DATA));
    if(xQueueDistance == NULL)
    {
        ERROR;
    }
}

int sendBlockMsgToDistanceQ1(DISTANCE_DATA *data)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_DISTANCE);
    success = xQueueSendFromISR(xQueueDistance, (void *) data, pdFALSE);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_DISTANCE);
    return ret;
}

int recieveMsgFromDistanceQ1(DISTANCE_DATA *data, int * transfer) {
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    DISTANCE_DATA received;
    success = xQueueReceive(xQueueDistance, &received, portMAX_DELAY);

    if(success == pdFALSE)
    {
        ret = -1;
    } else {
        *data = received;
        *transfer = 1;
    }

    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
    return ret;
}
