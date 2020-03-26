/*
 * timer.h
 *
 *  Created on: Mar 16, 2020
 *      Author: Holden Ramsey
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <ti/drivers/Timer.h>
#include "mqtt_queue.h"

#define TIMER_PERIOD (100000)

void timerCallback(Timer_Handle myHandle);
void timerInit();

#endif /* TIMER_H_ */
