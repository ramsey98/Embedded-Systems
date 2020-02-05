/*
 * sensor_queue.h
 *
 *  Created on: Jan 25, 2020
 *      Author: Jon Glaser
 */

#ifndef SENSOR_QUEUE_H_
#define SENSOR_QUEUE_H_

#include "debug.h"
#include "queue.h"

unsigned int itemToReceive;
int sendTimeMsgToQ1(unsigned int timeVal);
int sendSensorMsgToQ1(int mmDist);
int receiveFromQ1();
void createSensorQueue();

#endif /* SENSOR_QUEUE_H_ */
