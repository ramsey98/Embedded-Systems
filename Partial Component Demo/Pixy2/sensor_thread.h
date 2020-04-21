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
#define SENSOR_OFFSET 7.12 //sensor is currently 7 cm closer than pixy due to setup limitations

void sendSensorStateUSInfo(int distance);
void *sensorThread(void *arg0);

#endif /* SENSOR_THREAD_H_ */
