/*
 * timertwo.h
 *
 *  Created on: Jan 25, 2020
 *      Author: Jon Glaser
 */

#ifndef TIMERTWO_H_
#define TIMERTWO_H_

#include <ti/drivers/Timer.h>
#include <ti/drivers/ADC.h>

#include "debug.h"
#include "sensor_queue.h"

//ADC variables
int res;
uint16_t adcValue;
uint32_t result;
ADC_Handle   adc;
ADC_Params   adc_params;

//Timer variables
Timer_Handle timer1;
Timer_Params timer_params;

//Timer functions
void timer75Callback(Timer_Handle myHandle);
void timerTwoInit();

//ADC function
int conversion(ADC_Handle adc);

#endif /* TIMERTWO_H_ */
