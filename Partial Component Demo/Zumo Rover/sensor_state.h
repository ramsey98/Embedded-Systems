/*
 * sensor_state.h
 *
 *  Created on: Jan 25, 2020
 *      Author: Team 20
 */

#ifndef SENSOR_STATE_H_
#define SENSOR_STATE_H_

#include "debug.h"

typedef enum
{
    Init,
    Polling,
    Averaging,
} SENSOR_STATES;

typedef struct
{
    SENSOR_STATES state;
    int sensorTotal, sensorCount, sensorAvg;

} SENSOR_DATA;

int fsm(SENSOR_DATA *curState, int sensorVal);

#endif /* SENSOR_STATE_H_ */
