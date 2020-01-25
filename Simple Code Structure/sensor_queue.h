/*
 * sensor_queue.h
 *
 *  Created on: Jan 25, 2020
 *      Author: Jon Glaser
 */

#ifndef SENSOR_QUEUE_H_
#define SENSOR_QUEUE_H_

#include <ti/drivers/ADC.h>
#include "debug.h"

/* ADC conversion result variables */
uint16_t adcValue;
uint32_t adcValueMicroVolt;

int sendTimeMsgToQ1(unsigned int timeVal);
int sendSensorMsgToQ1(int mmDist);


#endif /* SENSOR_QUEUE_H_ */
