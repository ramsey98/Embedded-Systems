/*
 * sensor_queue.h
 *
 *  Created on: Jan 25, 2020
 *      Author: Team 20
 */

#ifndef SENSOR_QUEUE_H_
#define SENSOR_QUEUE_H_

#include "debug.h"
#include "queue.h"

void sendSensorMsgToQ(uint16_t mmDist);
void receiveFromSensorQ(uint16_t * sensorVal);
void createSensorQueue();

#endif /* SENSOR_QUEUE_H_ */
