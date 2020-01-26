/*
 * sensor_state.h
 *
 *  Created on: Jan 25, 2020
 *      Author: Jon Glaser
 */

#ifndef SENSOR_STATE_H_
#define SENSOR_STATE_H_

#include <ti/drivers/GPIO.h>
#include "ti_drivers_config.h"
#include "debug.h"


typedef enum
{
    Init,
    WaitingForTime1,
    WaitingForTime2,
    WaitingForTime3,
} STATES;

int curTime, sensorTotal, sensorCount, sensorAvg;
STATES curState;

int fsm(STATES curState, int timeInc, int sensorVal);

#endif /* SENSOR_STATE_H_ */
