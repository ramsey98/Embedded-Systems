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

#define FMASK (0xffffffff)
#define TIMEMASK (0x00000001)
#define SENSORMASK (0x10000000)
#define TIMEFLAG (0x0000000100000000)
#define SENSORFLAG (0x1000000000000000)
#define SHIFT (32)

int sendTimeMsgToQ1(unsigned int timeVal);
int sendSensorMsgToQ1(int mmDist);
int receiveFromQ1(int * timeInc, int * sensorVal);
void createSensorQueue();

#endif /* SENSOR_QUEUE_H_ */
