/*
 * mqtt_queue.h
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#ifndef MQTT_QUEUE_H_
#define MQTT_QUEUE_H_

#include "debug.h"
#include "queue.h"

typedef struct
{
    char ID;
    uint8_t state;
    uint8_t left;
    uint8_t right;
} MQTTMsg;

#define MQTT_STATE (0)
#define MQTT_LEFTMOTOR (1)
#define MQTT_RIGHTMOTOR (2)

int sendStateMsgToMQTTQ(uint8_t state);
int sendLeftMotorMsgToMQTTQ(uint8_t leftmotor);
int sendRightMotorMsgToMQTTQ(uint8_t rightmotor);
int receiveFromMQTTQ(uint8_t *index, uint8_t *state, uint8_t *leftmotor, uint8_t *rightmotor);
void createMQTTQueue();


#endif /* MQTT_QUEUE_H_ */
