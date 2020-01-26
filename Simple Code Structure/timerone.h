/*
 * timerone.h
 *
 *  Created on: Jan 25, 2020
 *      Author: Jon Glaser
 */

#ifndef TIMERONE_H_
#define TIMERONE_H_

#include <ti/drivers/Timer.h>
#include "debug.h"
#include "sensor_queue.h"

Timer_Handle timer0;
Timer_Params params;

void timerSecondCallback(Timer_Handle myHandle);
void timerOneInit();

#endif /* TIMERONE_H_ */
