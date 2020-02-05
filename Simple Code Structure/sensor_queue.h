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

int sendTimeMsgToQ1(unsigned int timeVal);
int sendSensorMsgToQ1(int mmDist);
int receiveFromQ1(int * timeInc, int * sensorVal);
void createSensorQueue();

#endif /* SENSOR_QUEUE_H_ */
