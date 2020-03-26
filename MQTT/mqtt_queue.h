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
#include "definitions.h"

void sendMsgToMQTTQ(MQTTMsg msg);
void receiveFromMQTTQ(MQTTMsg *received);
void createMQTTQueue();

#endif /* MQTT_QUEUE_H_ */
