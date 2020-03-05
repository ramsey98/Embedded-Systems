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
#include "motor_encoder_queue.h"

//Timer functions
void timer75Callback(Timer_Handle myHandle);
Timer_Handle timerTwoInit();

//ADC function
int conversion(ADC_Handle adc);
void adcInit();

#endif /* TIMERTWO_H_ */
