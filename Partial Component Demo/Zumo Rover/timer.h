/*
 * timer.h
 *
 *  Created on: Feb 25, 2020
 *      Author: Holden Ramsey
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "debug.h"
#include "sensor.h"
#include "PID.h"
#include "pixy.h"
#include "capture.h"
#include <ti/drivers/Timer.h>

#define TIMER_PERIOD (100000)

void timerCallback(Timer_Handle myHandle);
void timerInit();

#endif /* TIMER_H_ */
