/*
 * sensorstate_thread.h
 *
 *  Created on: Apr 17, 2020
 *      Author: Matthew
 */

#ifndef SENSORSTATE_THREAD_H_
#define SENSORSTATE_THREAD_H_

#include "debug.h"
#include "sensorstate_queue.h"
#include "mqtt_queue.h"

#define SENSOR_OFFSET 5 //sensor is currently 5 cm closer than pixy due to setup limitations

typedef enum {
    None,
    RedEgg,
    GreenEgg,
    YellowEgg,
    Zumo,
    Obstacle
} OBJECT_TYPES;

typedef enum {
    Left,
    Center,
    Right
} OBJECT_POSITION;


void *sensorStateThread(void *arg0);
void determineSensorState(SENSORSTATE_QUEUE_DATA * sensorData, SENSORSTATE_QUEUE_DATA * pixyData);

#endif /* SENSORSTATE_THREAD_H_ */
