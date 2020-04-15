/*
 * mqtt_queue.h
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#ifndef MQTT_QUEUE_H_
#define MQTT_QUEUE_H_

#include "FreeRTOS.h"
#include "queue.h"
#include "debug.h"

typedef struct
{
    uint8_t topic;
    uint8_t type;
    uint32_t value;
} MQTTMsg;

void sendMsgToMQTTQ(MQTTMsg msg);
void sendMsgToMQTTQFromISR(MQTTMsg msg);
void receiveFromMQTTQ(MQTTMsg *received);
void createMQTTQueue();

#endif /* MQTT_QUEUE_H_ */
