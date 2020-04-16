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
    int type = 0, value = 0;
    MQTTMsg msg;
    while(1)
    {
        receiveFromConfigQ(&msg);
        type = msg.type;
        value = msg.value;
        switch(type)
        {
            case CONFIG_STATE:
                if(value == ROVER_LOADING)
                {
                    sendMsgToNaviQ(PAUSE, EMPTY);
                }
                else if(value == ROVER_MOVING)
                {
                    sendMsgToNaviQ(RESUME, EMPTY);
                }
                break;
            case CONFIG_PID_ENABLE:
                sendMsgToNaviQ(PID_ENABLE, value);
                break;
            case CONFIG_SET_SPEED:
                sendMsgToNaviQ(FORWARD, value);
                break;
            case CONFIG_TURN_RIGHT:
                sendMsgToNaviQ(TURNRIGHT, value);
                break;
            case CONFIG_TURN_LEFT:
                sendMsgToNaviQ(TURNLEFT, value);
                break;
            case CONFIG_SENSOR_ENABLE:
                sendMsgToNaviQ(SENSOR_ENABLE, value);
                break;
            case CONFIG_PIXY_ENABLE:
                sendMsgToNaviQ(PIXY_ENABLE, value);
                break;
            default:
                break;
        }
    }
}

void createConfigQueue()
{
    xQueue = xQueueCreate(16, sizeof(MQTTMsg));
    if(xQueue == NULL) ERROR;
}

void sendMsgToConfigQFromISR(MQTTMsg msg)
{
    dbgOutputLoc(BEFORE_SEND_QUEUE_ISR_TIMER);
    BaseType_t success = xQueueSendFromISR(xQueue, (void *) &msg, pdFALSE);
    if(success == pdFALSE) ERROR;
    dbgOutputLoc(AFTER_SEND_QUEUE_ISR_TIMER);
}

void receiveFromConfigQ(MQTTMsg *received)
{
    dbgOutputLoc(BEFORE_RECEIVE_QUEUE);
    MQTTMsg temp;
    BaseType_t success = xQueueReceive(xQueue, &temp, portMAX_DELAY);
    if(success == pdFALSE) ERROR;
    *received = temp;
    dbgOutputLoc(AFTER_RECEIVE_QUEUE);
}


