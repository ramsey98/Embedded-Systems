/*
 * sensor.h
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include <ti/drivers/Timer.h>
#include <ti/drivers/ADC.h>

#include "debug.h"
#include "sensor_queue.h"
#include "sensor_state.h"

#define TIMER_PERIOD (100000)

//Timer functions
void *sensorThread(void *arg0);
void timerCallback(Timer_Handle myHandle);
void timerInit();

//ADC function
int conversion(uint32_t sensorVal);
void adcInit();

#endif /* SENSOR_H_ */
