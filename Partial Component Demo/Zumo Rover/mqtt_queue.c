/*
 * mqtt_queue.c
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#include "MQTT_queue.h"
static QueueHandle_t xQueue = NULL;

void createMQTTQueue()
{
    xQueue = xQueueCreate(16, sizeof(MQTTMsg));
    if(xQueue == NULL)
    {
        halt();
    }
}

int sendStateMsgToMQTTQ(uint8_t state)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    MQTTMsg msg = {MQTT_STATE, state, NULL, NULL};
    success = xQueueSend(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER1);
    return ret;
}

int sendLeftMotorMsgToMQTTQ(uint16_t leftmotor)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    MQTTMsg msg = {MQTT_LEFTMOTOR, NULL, leftmotor, NULL};
    success = xQueueSend(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER1);
    return ret;
}

int sendRightMotorMsgToMQTTQ(uint16_t rightmotor)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    MQTTMsg msg = {MQTT_RIGHTMOTOR, NULL, NULL, rightmotor};
    success = xQueueSend(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER1);
    return ret;
}

int receiveFromMQTTQ(uint8_t *index, uint8_t *state, uint16_t *leftmotor, uint16_t *rightmotor)
{
    int ret = 0;
    BaseType_t success;
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    MQTTMsg received;
    success = xQueueReceive(xQueue, &received, portMAX_DELAY);
    if(success == pdFALSE)
    {
        ret = -1;
    }
    else
    {
        switch(received.ID)
        {
        case MQTT_STATE:
            *state = received.item1;
            *index = received.ID;
            break;
        case MQTT_LEFTMOTOR:
            *leftmotor = received.item2;
            *index = received.ID;
            break;
        case MQTT_RIGHTMOTOR:
            *rightmotor = received.item3;
            *index = received.ID;
            break;
        default:
            halt();
            break;
        }
    }
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
    return ret;
}


