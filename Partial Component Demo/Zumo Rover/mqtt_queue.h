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

typedef struct {
    char ID;
    uint8_t item1;
    uint16_t item2;
    uint16_t item3;
} MQTTMsg;

#define MQTT_STATE (0)
#define MQTT_LEFTMOTOR (1)
#define MQTT_RIGHTMOTOR (2)

int sendMsgToMQTTQ(uint8_t state);
int sendLeftMotorMsgToMQTTQ(uint16_t leftmotor);
int sendRightMotorMsgToMQTTQ(uint16_t rightmotor);
int receiveFromMQTTQ(uint8_t *index, uint8_t *state, uint16_t *leftmotor, uint16_t *rightmotor);
void createMQTTQueue();


#endif /* MQTT_QUEUE_H_ */
