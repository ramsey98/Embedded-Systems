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
    uint8_t type;
    uint8_t value1;
    uint8_t value2;
    uint8_t value3;
} MQTTMsg;

void sendMsgToMQTTQ(MQTTMsg msg);
void sendMsgToMQTTQFromISR(MQTTMsg msg);
void receiveFromMQTTQ(MQTTMsg *received);
void createMQTTQueue();

#endif /* MQTT_QUEUE_H_ */
