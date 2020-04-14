/*
 * capture.h
 *
 *  Created on: Feb 17, 2020
 *      Author: Matthew Conway
 */

#ifndef CAPTURE_H_
#define CAPTURE_H_

#include "sensor_queue.h"
#include "debug.h"
#include <ti/drivers/Capture.h>

void captureCallback(Capture_Handle handle, uint32_t interval);
void captureInit();

#endif /* CAPTURE_H_ */
