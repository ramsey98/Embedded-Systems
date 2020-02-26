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
        ERROR;
    }
}

void sendMsgToMQTTQ(uint8_t type, uint8_t value)
{
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER1);
    MQTTMsg msg;
    switch(type)
    {
        case MQTT_STATE:
        {
            msg.ID = MQTT_STATE;
            msg.state = value;
            break;
        }
        case MQTT_LEFTMOTOR:
        {
            msg.ID = MQTT_LEFTMOTOR;
            msg.left = value;
            break;
        }
        case MQTT_RIGHTMOTOR:
        {
            msg.ID = MQTT_RIGHTMOTOR;
            msg.right = value;
            break;
        }
        default:
        {
            ERROR;
        }
    }
    BaseType_t success = xQueueSend(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE)
    {
        ERROR;
    }
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER1);
}

void receiveFromMQTTQ(uint8_t *index, uint8_t *state, uint8_t *leftmotor, uint8_t *rightmotor)
{
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    MQTTMsg received;
    BaseType_t success = xQueueReceive(xQueue, &received, portMAX_DELAY);
    if(success == pdFALSE)
    {
        ERROR;
    }
    else
    {
        switch(received.ID)
        {
        case MQTT_STATE:
            *state = received.state;
            break;
        case MQTT_LEFTMOTOR:
            *leftmotor = received.left;
            break;
        case MQTT_RIGHTMOTOR:
            *rightmotor = received.right;
            break;
        default:
            ERROR;
            break;
        }
        *index = received.ID;
    }
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
}


