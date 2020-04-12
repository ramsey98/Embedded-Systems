/*
 * timer.h
 *
 *  Created on: Mar 16, 2020
 *      Author: Holden Ramsey
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <navigation.h>
#include <ti/drivers/Timer.h>
#include "debug.h"
#include "sensor.h"
#include "pixy.h"
#include "capture.h"
#include "mqtt_queue.h"
#include "json_parse.h"

#define TIMER_PERIOD (100000)

void timerCallback(Timer_Handle myHandle);
void timerInit();

#endif /* TIMER_H_ */
