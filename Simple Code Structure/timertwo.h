/*
 * timertwo.h
 *
 *  Created on: Jan 25, 2020
 *      Author: Team 20
 */

#ifndef TIMERTWO_H_
#define TIMERTWO_H_

#include <ti/drivers/Timer.h>
#include <ti/drivers/ADC.h>

#include "debug.h"
#include "sensor_queue.h"

#define TIMER1_PERIOD (75000)

//Timer functions
void timer75Callback(Timer_Handle myHandle);
void timerTwoInit();

//ADC function
int conversion(uint32_t sensorVal);
void adcInit();

#endif /* TIMERTWO_H_ */
