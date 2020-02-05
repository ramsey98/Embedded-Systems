/*
 * sensor_state.h
 *
 *  Created on: Jan 25, 2020
 *      Author: Team 20
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
} SENSOR_STATES;

typedef struct
{
    SENSOR_STATES state;
    int curTime, sensorTotal, sensorCount, sensorAvg;

} SENSOR_DATA;

int fsm(SENSOR_DATA *curState, int timeInc, int sensorVal);

#endif /* SENSOR_STATE_H_ */
