/*
 * sensor_thread.h
 *
 *  Created on: Apr 12, 2020
 *      Author: Matthew
 */

#ifndef SENSOR_THREAD_H_
#define SENSOR_THREAD_H_

#include "debug.h"
#include "sensor_queue.h"
#include "timertrigger.h"
#include "mqtt_queue.h"
#include "sensorstate_queue.h"

#define CM_PER_US (0.0343)

void sendSensorStateUSInfo(int distance);
void *sensorThread(void *arg0);

#endif /* SENSOR_THREAD_H_ */
