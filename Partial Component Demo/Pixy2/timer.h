/*
 * timertwo.h
 *
 *  Created on: Jan 25, 2020
 *      Author: Team 20
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <ti/drivers/Timer.h>
#include <ti/drivers/ADC.h>

#include "debug.h"
#include "sensor_queue.h"
#include "pixy_queue.h"
#include "mqtt_queue.h"
#include "json_parse.h"

#define TIMER100_PERIOD (100000)

//Timer functions
void timer100MSCallback(Timer_Handle myHandle);
void timer100MSInit();

#endif /* TIMER_H_ */
