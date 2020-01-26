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


int res;
void timer75Callback(Timer_Handle myHandle);
int conversion(ADC_Handle adc);
void timerTwoInit();


#endif /* TIMERTWO_H_ */
