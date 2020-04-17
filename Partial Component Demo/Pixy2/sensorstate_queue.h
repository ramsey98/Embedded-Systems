/*
 * sensorstate_queue.h
 *
 *  Created on: Apr 17, 2020
 *      Author: Matthew
 */

#ifndef SENSORSTATE_QUEUE_H_
#define SENSORSTATE_QUEUE_H_

#include "debug.h"
#include "queue.h"
#include "spi.h"

typedef enum {
    ultrasonic_data,
    pixy2_data
} SENSORSTATE_DATA_TYPES;

typedef struct {
    int type;
    int distance;
    int color;
    int offset;
} SENSORSTATE_QUEUE_DATA;

void createSensorStateQueue();
void sendSensorStateMsgToQ(SENSORSTATE_QUEUE_DATA * data);
void receiveFromSensorStateQ(SENSORSTATE_QUEUE_DATA * data);

#endif /* SENSORSTATE_QUEUE_H_ */
