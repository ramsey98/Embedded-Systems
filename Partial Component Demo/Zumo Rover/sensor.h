/*
 * sensor.h
 *
 *  Created on: Feb 18, 2020
 *      Author: Holden Ramsey
 */

#ifndef SENSOR_H_
#define SENSOR_H_


#include <navi_queue.h>
#include <ti/drivers/ADC.h>
#include "debug.h"

#define DICTLEN (15)

typedef struct
{
    int val;
    int dist;
} lookupTable;

void *sensorThread(void *arg0);
void adcInit();
void pollSensor();
int conversion(uint32_t sensorVal);

#endif /* SENSOR_H_ */
