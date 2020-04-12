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
#include "uart_term.h"

#define SENSOR_TYPE_TRIGGER (1)
#define SENSOR_TYPE_SUM (2)

void createSensorQueue();
int sendSensorMsgToQ(int value);
int receiveFromSensorQ();

#endif /* SENSOR_QUEUE_H_ */
