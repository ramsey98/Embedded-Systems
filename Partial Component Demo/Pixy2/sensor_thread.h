/*
 * sensor_thread.h
 *
 *  Created on: Apr 12, 2020
 *      Author: Matthew
 */

#ifndef SENSOR_THREAD_H_
#define SENSOR_THREAD_H_

#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/Capture.h>

#include "debug.h"
#include "sensor_queue.h"
#include "timertrigger.h"

#define CM_PER_US (12)

volatile uint32_t curInterval;
static SemaphoreP_Handle captureSem;

void *sensorThread(void *arg0);
void captureCallback(Capture_Handle handle, uint32_t interval);

#endif /* SENSOR_THREAD_H_ */
