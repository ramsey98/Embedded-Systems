/*
 * sensorstate_queue.c
 *
 *  Created on: Apr 17, 2020
 *      Author: Matthew
 */

#include "sensorstate_queue.h"

static QueueHandle_t xQueueSensorState = NULL;

void createSensorStateQueue()
{
    xQueueSensorState = xQueueCreate(16, sizeof(SENSORSTATE_QUEUE_DATA));
    if(xQueueSensorState == NULL) ERROR;
}

void sendSensorStateMsgToQ(SENSORSTATE_QUEUE_DATA * data)
{
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_SENSOR_STATE);
    success = xQueueSendFromISR(xQueueSensorState, (void *) data, pdFALSE);
    if(success == pdFALSE) ERROR;
    dbgOutputLoc(AFTER_SEND_QUEUE_SENSOR_STATE);
}

void receiveFromSensorStateQ(SENSORSTATE_QUEUE_DATA * data)
{
    BaseType_t success;
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    SENSORSTATE_QUEUE_DATA received;
    success = xQueueReceive(xQueueSensorState, &received, portMAX_DELAY);

    if(success == pdFALSE)
    {
        ERROR;
    } else {
        *data = received;
    }

   dbgOutputLoc(AFTER_RECEIVE_QUEUE);
}
