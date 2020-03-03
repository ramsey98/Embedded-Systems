/*
 * timerone.h
 *
 *  Created on: Jan 25, 2020
 *      Author: Team 20
 */

#ifndef TIMERONE_H_
#define TIMERONE_H_

#include <ti/drivers/Timer.h>
#include "debug.h"
#include "sensor_queue.h"
#include "pixy_queue.h"

#define TIMER0_PERIOD (1000000)

void timerSecondCallback(Timer_Handle myHandle);
void timerOneInit();

#endif /* TIMERONE_H_ */
